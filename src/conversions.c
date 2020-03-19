/*Conversions.c*/
/**
 * This file contains functions which convert from php zval/HashTable to as_val types
 * and vice versa. These are internal to the extension and should not be exposed directly
 * to the user.
 */
#include "conversions.h"
#include <stdbool.h>
#include "ext/standard/php_var.h"
#include <zend_smart_str.h>
#include "serializers.h"
#include "php_aerospike_types.h"
#include "aerospike/as_record_iterator.h"
#include "aerospike/as_bin.h"

#define BYTES_CLASS_NAME "Aerospike\\Bytes"
#define BYTES_STR_PROPERTY "s"
#define BYTES_STR_PROPERTY_LEN strlen(BYTES_STR_PROPERTY)

#define GEOJSON_CLASS_NAME "Aerospike\\GeoJSON"
#define GEOJSON_CLASS_TO_STR "__toString"
#define GEOJSON_STATIC_CONSTRUCTOR "fromJson"


/**
 * Data used to facilitate conversion of an as_list to a php array
 * Utilized as an argument to as_list_foreach
 *
 */
typedef struct _as_list_to_zval_data {
	/*
	 * Converted list so far
	 */
	zval* z_list;
	/*
	 * Index of next item to convert
	 */
	long index;
	/*
	 * Error to be populated in case of failure
	 */
	as_error* err;
}as_list_to_zval_data;

/**
 * Data used to facilitate conversion of as_map to zval
 */
typedef struct _as_map_to_zval_data {
	zval* z_map;
	as_error* err;
}as_map_to_zval_data;


/**
 * Function passed as a callback to as_map_foreach. It will convert the map
 * into a PHP Array. If the as_map keys are not strings or integers, an error will be raised.
 * In the case of a CDT value, this may be called recursively.
 *
 * @param key		The key in the key/value pair of the as_map. Must be string or long
 * @param value		The value in the key/value pair. May be any of the as_val types
 * @param udata		Struct holding an _as_map_to_zval_data struct. Contains an error
 * 					to be set in case of failure, and a zval* containing the transformed data
 * 					so far
 *
 * @return On success true, otherwise false. false halts the callback to this function. Error will
 * 			be set on failure.
 */
bool as_map_to_zval_foreach_converter(const as_val* key, const as_val* value, void* udata) {
	as_map_to_zval_data* conversion_data = (as_map_to_zval_data*)udata;
	zval key_zval;
	// Initialize the zval to an empty value
	ZVAL_NULL(&key_zval);

	as_val_to_zval(key, &key_zval, conversion_data->err);
	if (Z_TYPE_P(&key_zval) != IS_STRING && Z_TYPE_P(&key_zval) != IS_LONG) {
		as_error_set_message(conversion_data->err, AEROSPIKE_ERR_CLIENT,
							"Hashtable keys must be strings or integers");
		zval_dtor(&key_zval);
		return false;
	}

	zval store_zval;
	ZVAL_UNDEF(&store_zval);

	as_val_to_zval(value, &store_zval, conversion_data->err);

	if (conversion_data->err->code != AEROSPIKE_OK) {
		zval_dtor(&store_zval);
		zval_dtor(&key_zval);
		return false;
	}

	if (Z_TYPE_P(&key_zval) == IS_STRING) {
		add_assoc_zval(conversion_data->z_map, Z_STRVAL_P(&key_zval), &store_zval);
	// key must be a string or a long, and it is not a string, so it is a long
	} else {
		add_index_zval(conversion_data->z_map, Z_LVAL_P(&key_zval), &store_zval);
	}
	// Need to free the zval we used as a key just in case it had allocated memory as well
	// It is stack allocated, so use zval_dtor.
	zval_dtor(&key_zval);
	return true;
}


/**
 * Convert an as_map to a zval. Utilizes as_map_foreach to do most of the work.
 *
 * @param aerospike_map		map to be converted to php type
 * @param z_map				php zval which will hold the converted data
 * @param err				error structure to be filled in case of failure
 * @param serializer_type	enum value indicating which type of serialization will be utilized.
 * @return true on success, false on failure
 */
as_status as_map_to_zval(as_map* aerospike_map, zval* z_map, as_error* err) {
	array_init(z_map);

	//setup the udata for the foreach call
	as_map_to_zval_data conversion_data;
	conversion_data.z_map = z_map;
	conversion_data.err = err;

	as_map_foreach(aerospike_map, as_map_to_zval_foreach_converter, (void*)&conversion_data);
	if (err->code != AEROSPIKE_OK) {
		// Failed to convert, must destroy the old value
		zval_dtor(z_map);
		ZVAL_UNDEF(z_map);
		return err->code;
	}
	return AEROSPIKE_OK;
}


/**
 * Convert as_record to a z_val. This produces a php array of the type ["bins"=>[...], "metadata"=>[...], "key"=>[...]]
 *
 * @param aerospike_record	the as_record to convert
 * @param z_record			the php struct to hold the converted record. It will be setup and destructed on failure.
 * @param record_key		the key used to fetch the record, NULL if called from a query/scan callback
 * @param show_pk			whether the returned key array should include the primary key or a NULL
 * @param err				error to be set in case of failure
 */
as_status as_record_to_zval(const as_record* aerospike_record, zval* z_record, const as_key* record_key, bool show_pk, as_error* err) {
	zval z_bins;
	ZVAL_UNDEF(&z_bins);
	zval z_key;
	ZVAL_UNDEF(&z_key);
	zval z_meta;
	ZVAL_UNDEF(&z_meta);
	const as_key* conversion_key = NULL;

	array_init(z_record);
	// This should never happen, but it is handled just in case
	if (Z_TYPE_P(z_record) != IS_ARRAY) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "ZVAL passed to as_record_to_zval is not an Array");
		return err->code;
	}

	if (as_bins_to_zval(aerospike_record, &z_bins, err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	array_init(&z_meta);
	as_record_to_z_metadata(aerospike_record, &z_meta, err);
	if(err->code != AEROSPIKE_OK) {
		zval_dtor(&z_bins);
		zval_dtor(&z_meta);
		goto CLEANUP;
	}

	
	// If record_key is specified, use it to fill in the zval key
	// otherwise use the key stored in the as_record
	conversion_key = record_key ? record_key : &aerospike_record->key;

	as_key_to_zval(conversion_key, &z_key, show_pk, err);

	if(err->code != AEROSPIKE_OK) {
		zval_dtor(&z_bins);
		zval_dtor(&z_meta);
		goto CLEANUP;
	}

	add_assoc_zval(z_record, "key", &z_key);
	add_assoc_zval(z_record, "metadata", &z_meta);
	add_assoc_zval(z_record, "bins", &z_bins);

CLEANUP:
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_record);
		ZVAL_NULL(z_record);
	}
	return err->code;
}


as_status as_bins_to_zval(const as_record* aerospike_record, zval* z_bins, as_error* err) {

	as_bin* bin = NULL;
	as_bin_value* bin_val = NULL;

	zval z_bin_value;
	ZVAL_NULL(&z_bin_value);

	array_init(z_bins);

	as_record_iterator it;
	as_record_iterator_init(&it, aerospike_record);

	while (as_record_iterator_has_next(&it)) {
		bin = as_record_iterator_next(&it);
		if (!bin) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Null bin");
			goto CLEANUP;
		}
		bin_val = as_bin_get_value(bin);
		if (!bin_val) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Null bin value");
			goto CLEANUP;
		}
		if (as_val_to_zval((as_val*)bin_val, &z_bin_value, err) != AEROSPIKE_OK) {
			/* In case of error this will have set the err code, so don't reset it here */
			goto CLEANUP;
		}
		if (add_assoc_zval(z_bins, bin->name, &z_bin_value) != SUCCESS) {
			zval_dtor(&z_bin_value);
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add zval to hashtable");
			goto CLEANUP;
		}
	}

CLEANUP:
	as_record_iterator_destroy(&it);
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_bins);
		ZVAL_NULL(z_bins);
	}
	return err->code;
}

/**
 *	callback function used to transform as_list into a zval array
 *	@param bin_value		The as_value to be converted
 *	@param udata			as_list_to_zval_data struct containing information on the transformation
 *							such as index of next transformation, and error to be set on failure
 *
 *	@return					true to indicate success and continue transformation, false to indicate failure
 *							and halt transformation
**/
bool as_list_to_zval_foreach_converter(as_val* bin_value, void* udata) {
	as_list_to_zval_data* conversion_data = (as_list_to_zval_data*) udata;
	zval z_bin_val;
	ZVAL_UNDEF(&z_bin_val);

	as_val_to_zval((const as_val*)bin_value, &z_bin_val, conversion_data->err); // Better error handling here, should be converter with bool
	if(conversion_data->err->code != AEROSPIKE_OK) {
		zval_dtor(&z_bin_val);
		return false;
	}
	add_index_zval(conversion_data->z_list, conversion_data->index, &z_bin_val);
	conversion_data->index++;
	return true;
}


/**
 * Convert an as_list to a zval array
 *
 * @param list				The as_list to convert from
 * @param z_list			The zval to be converted to an array and to hold the converted data.
 * @param error				The error which will be filled in case of failure
 * @param serializer_type	The type of serializer to be used to convert into the zval
 *
 * @return true on success, false otherwise. In case of failure, err will be populated.
 */
as_status as_list_to_zval(as_list* list, zval* z_list, as_error* err) {
	as_list_to_zval_data conversion_data;
	conversion_data.index = (long)0;
	array_init(z_list);
	conversion_data.z_list = z_list;
	conversion_data.err = err;

	as_list_foreach(list, as_list_to_zval_foreach_converter, (void*)&conversion_data);
	if (err->code != AEROSPIKE_OK) {
		return err->code;
	}
	return AEROSPIKE_OK;
}


/**
 * Top level conversion for converting from PHP types to as_val types.
 * In case of non CDT types, a direct conversion will occur, otherwise
 * this will call as_list_to_zval/ as_map_to_zval, and this function may be
 * called recursively. Non natively supported types will be passed to a proper serializer function.
 *
 * @param zval_to_convert	The PHP value to be converted
 * @param err				The error to be populated on failure
 * @param serializer_type	The type of serializer to use, one of SERIALIZER_PHP, SERIALIZER_NONE, SERIALIZER_USER
 *
 * @return An allocated pointer to as_val.
 *
 */
as_status zval_to_as_val(zval* zval_to_convert, as_val** val, as_error* err, int serializer_type) {
	*val = NULL;

	switch (Z_TYPE_P(zval_to_convert)) {
		// ZVAL Long convert to as_integer
		case IS_LONG: {
			as_integer* converted_integer = NULL;
			int64_t temp_int64;
			temp_int64 = (int64_t)Z_LVAL_P(zval_to_convert);
			converted_integer = as_integer_new(temp_int64);
			*val = as_integer_toval(converted_integer);
			break;
		}
		case IS_STRING: {
			as_string* converted_string = NULL;
			char* temp_str = NULL;
			temp_str = strdup(Z_STRVAL_P(zval_to_convert));
			// Just in case the zval changes or stops existing mid request,
			// copy it before storing.
			converted_string = as_string_new(temp_str, true);
			*val = as_string_toval(converted_string);
			break;
		}
		case IS_DOUBLE: {
			double temp_double;
			as_double* converted_double = NULL;
			temp_double = Z_DVAL_P(zval_to_convert);
			converted_double = as_double_new(temp_double);
			*val = as_double_toval(converted_double);
			break;
		}
		case IS_ARRAY: {
			// NEED MORE ERROR CHECKING HERE
			if(hashtable_is_list(Z_ARRVAL_P(zval_to_convert))) {
				as_list* converted_list = NULL;
				if (z_hashtable_to_as_list(
						Z_ARRVAL_P(zval_to_convert), &converted_list, err, serializer_type) == AEROSPIKE_OK){
					*val = as_list_toval(converted_list);
				}
			} else {
				as_map* converted_map = NULL;
				if (z_hashtable_to_as_map(
						Z_ARRVAL_P(zval_to_convert), &converted_map, err, serializer_type) == AEROSPIKE_OK) {
					*val = as_map_toval(converted_map);
				}
			}
			break;
		}
		default: {

			// CHECK FOR BYTES AND GEOJSON
			if (Z_TYPE_P(zval_to_convert) == IS_OBJECT) {
				zend_object* store_obj = Z_OBJ_P(zval_to_convert);
				zend_class_entry* ce = store_obj->ce;
				char* class_name = ce->name->val;

				size_t class_name_len = ce->name->len;
				if (strncmp(class_name, BYTES_CLASS_NAME, class_name_len) == 0) {
					zval* bytes_str = NULL;
					zval* return_val = NULL;
					bytes_str = zend_read_property(ce, zval_to_convert, BYTES_STR_PROPERTY, BYTES_STR_PROPERTY_LEN, 1, return_val);

					if (!bytes_str) {
						as_error_update(err, AEROSPIKE_ERR_PARAM, "Unable to convert value to as type");
						return err->code;
					}

					if (Z_TYPE_P(bytes_str) != IS_STRING) {
						as_error_update(err, AEROSPIKE_ERR_PARAM, "Type of bytes is non string");
						return err->code;
					}
					as_bytes* bytes_blob = as_bytes_new(Z_STRLEN_P(bytes_str));
					as_bytes_set_type(bytes_blob, AS_BYTES_BLOB);
					as_bytes_set(bytes_blob, 0, (uint8_t*)Z_STRVAL_P(bytes_str), Z_STRLEN_P(bytes_str));
					*val = as_bytes_toval(bytes_blob);
					return err->code;

				} else if (strncmp(class_name, GEOJSON_CLASS_NAME, class_name_len) == 0) {
					zval retval;
					ZVAL_NULL(&retval);
					zval to_string_name;
					ZVAL_STRING(&to_string_name, GEOJSON_CLASS_TO_STR);

					if (call_user_function(EG(function_table), zval_to_convert, &to_string_name,
							&retval, 0, NULL) == FAILURE) {
						zval_dtor(&to_string_name);
						as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to convert geojson");
						return err->code;
					}

					zval_dtor(&to_string_name);

					if (Z_TYPE_P(&retval) != IS_STRING) {
						zval_dtor(&retval);
						as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to convert geojson");
						return err->code;
					}
					/* since we need to free the return value to prevent a memory leak,
					 * we need to copy the string. Because it could have a null byte in the
					 * middle, it's necessary to memcpy.
					 */
					char* geojson_str = malloc(Z_STRLEN_P(&retval) + 1);
					memcpy(geojson_str, Z_STRVAL_P(&retval), Z_STRLEN_P(&retval));
					geojson_str[Z_STRLEN_P(&retval)] = '\0';

					as_geojson* geojson = as_geojson_new_wlen(geojson_str,
							Z_STRLEN_P(&retval), true);

					*val = as_geojson_toval(geojson);
					zval_dtor(&retval);
					return err->code;
				}

			}

			as_bytes* bytes = NULL;
			serialize_php_object(zval_to_convert, &bytes, err, serializer_type);
			if (bytes) {
				*val = as_bytes_toval(bytes);
				return AEROSPIKE_OK;
			} //err here probably
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Unable to convert value to as type");
			return err->code;
		}
	}
	return err->code;
}

/**
  *Takes a php hashtable as an argument and stores it in the c_map
**/
/**
 * Function to convert a PHP array into an as_map. NOTE: this take a HashTable* instead of a zval, this allows
 * an error to be caught before this function is called, may be refactored later
 *
 * @param php_hash			the php associative array to be converted
 * @param c_map				the as_hashmap to be allocated and populated by the operation
 * @param err				Error structure to be filled in case of failure
 * @param serializer_type	Type of serializer to be used for non CDT values
 *
 * @return true on success, else false. Error will be filled on failure. Caller is responsible for cleaning up
 * the as_hashmap on failure
 */
as_status z_hashtable_to_as_map(HashTable* php_hash, as_map** c_map, as_error* err, int serializer_type) {
	*c_map = NULL;
	uint32_t number_of_elements = zend_hash_num_elements(php_hash);
	*c_map = (as_map*)as_hashmap_new(number_of_elements);

	if (*c_map == NULL) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to allocate memory for as_map");
		goto CLEANUP;
	}

	zend_ulong numeric_key;
	zend_string* string_key;
	zval* php_value;
	ZEND_HASH_FOREACH_KEY_VAL(php_hash, numeric_key, string_key, php_value)
	{
		as_val* as_val_key = NULL;

		// if key is non null, then there was a string key, else it is a long
		if (string_key) {
			as_string* as_string_key = as_string_new(strdup(ZSTR_VAL(string_key)), true);
			as_val_key = (as_val*)as_string_key;
		} else {
			as_integer* as_integer_key = as_integer_new((int64_t)numeric_key);
			as_val_key = (as_val*)as_integer_key;
		}

		as_val* as_val_value = NULL;
		zval_to_as_val(php_value, &as_val_value, err, serializer_type);
		// Convert the php value into an as_value;
		if (!as_val_value || err->code != AEROSPIKE_OK) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to convert value to aerospike map");
			return err->code;
		}

		as_map_set(*c_map, as_val_key, as_val_value);
	} ZEND_HASH_FOREACH_END();

CLEANUP:
	// Rollback allocation if creation failed.
	if(err->code != AEROSPIKE_OK) {
		if (*c_map) {
			as_map_destroy(*c_map);
			*c_map = NULL;
		}
	}
	return err->code;
}

/* Convert a php hashtable to an as_record */
/**
  *Takes a php hashtable as an argument and stores it in the c_map
**/
/**
 * Function to convert a PHP array into an as_record. NOTE: this take a HashTable* instead of a zval, this allows
 * an error to be caught before this function is called, may be refactored later
 *
 * @param php_hash			the php associative array to be converted
 * @param c_map				the as_record to be allocated and populated by the operation
 * @param err				Error structure to be filled in case of failure
 * @param serializer_type	Type of serializer to be used for non CDT values
 *
 * @return true on success, else false. Error will be filled on failure. Caller is responsible for cleaning up
 * the as_record
 */
as_status z_hashtable_to_as_record(HashTable* z_record_hash, as_record** record, as_error* err, int serializer_type) {
	uint32_t number_of_bins = zend_hash_num_elements(z_record_hash);
	*record = as_record_new(number_of_bins);
	zend_string* string_key;
	zval* php_value;

	ZEND_HASH_FOREACH_STR_KEY_VAL(z_record_hash, string_key, php_value) {
		// The record has a string key, this is valid
		if (string_key) {
			if ((ZSTR_LEN(string_key) > AS_BIN_NAME_MAX_LEN)) {
				as_error_update(err, AEROSPIKE_ERR_BIN_NAME, "Bin name too long");
				return err->code;
			}
			add_zval_to_record(php_value, *record, ZSTR_VAL(string_key), err, serializer_type);
			if (err->code != AEROSPIKE_OK) {
				return err->code;
			}
		} else {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Only string names are valid for bins");
			return err->code;
		}
	}  ZEND_HASH_FOREACH_END();

	// This should never be true at this point, but just in case
	if (err->code != AEROSPIKE_OK) {
		return err->code;
	}
	return AEROSPIKE_OK;
}


/**
 ** Function: as_val_to_zval
 ** {param} as_val* aerospike_value the as_val type to be convert to a matching PHP type
 ** {return} zval The PHP representation of the as_val
 ** This function takes an aerospike_value and returns the corresponding PHP object

**/
as_status as_val_to_zval(const as_val* aerospike_value, zval* return_val, as_error* err) {
	as_double* temp_double = NULL;
	as_integer* temp_integer = NULL;
	as_string* temp_string = NULL;
	as_list* temp_list = NULL;
	as_map* temp_map = NULL;
	as_bytes* temp_bytes = NULL;
	as_geojson* temp_geojson = NULL;
	int temp = as_val_type(aerospike_value);
	switch (temp) {
		case AS_STRING: {
			temp_string = as_string_fromval(aerospike_value);
			if(temp_string) {
				ZVAL_STRING(return_val, as_string_get(temp_string));
			} else {
				ZVAL_UNDEF(return_val);
			}
			break;
		}
		case AS_INTEGER: {
			temp_integer = as_integer_fromval(aerospike_value);
			if(temp_integer) {
				ZVAL_LONG(return_val, as_integer_get(temp_integer));
			} else {
				ZVAL_UNDEF(return_val);
			}
			break;
		}

		case AS_DOUBLE: {
			temp_double = as_double_fromval(aerospike_value);
			if(temp_double) {
				ZVAL_DOUBLE(return_val, as_double_get(temp_double));
			} else {
				ZVAL_UNDEF(return_val);
			}
			break;
		}
		case AS_LIST: {
			temp_list = as_list_fromval((as_val*)aerospike_value);
			as_list_to_zval(temp_list, return_val, err);
			break;
		}
		case AS_MAP: {
			temp_map = as_map_fromval(aerospike_value);
			as_map_to_zval(temp_map, return_val, err);
			break;
		}
		case AS_BYTES: {
			temp_bytes = as_bytes_fromval(aerospike_value);
			unserialize_as_bytes(temp_bytes, return_val, err);
			break;
		}
		case AS_GEOJSON: {
			temp_geojson = as_geojson_fromval(aerospike_value);
			as_geojson_to_zval(temp_geojson, return_val, err);
			break;
		}
		default:
			ZVAL_UNDEF(return_val);
			//error handling
			break;
	}
	if (err->code != AEROSPIKE_OK) {
		return err->code;
	}
	return AEROSPIKE_OK;

}

/*
 * Stores a PHP userland variable into an as_record
 */
as_status add_zval_to_record(zval* add_zval, as_record* record, const char* bin, as_error* err, int serializer_type) {
	
	as_val* as_val_to_add = NULL;

	if (zval_to_as_val(add_zval, &as_val_to_add, err, serializer_type) != AEROSPIKE_OK) {
		return err->code;
	}

	switch (as_val_type(as_val_to_add)) {

		case AS_INTEGER: {
			as_integer* store_int = as_integer_fromval(as_val_to_add);
			if (!store_int) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert integer");
				return err->code;
			}
			as_record_set_integer(record, bin, store_int);
			break;
		}
		case AS_DOUBLE: {
			as_double* store_double = as_double_fromval(as_val_to_add);

			if (!store_double) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert double");
				return err->code;
			}

			as_record_set_as_double(record, bin, store_double);
			break;
		}
		case AS_STRING: {
			as_string* store_string = as_string_fromval(as_val_to_add);

			if (!store_string) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert string");
				return err->code;
			}

			as_record_set_string(record, bin, store_string);
			break;
		}
		case AS_BYTES: {
			as_bytes* store_bytes = as_bytes_fromval(as_val_to_add);

			if (!store_bytes) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert bytes");
				return err->code;
			}

			as_record_set_bytes(record, bin, store_bytes);
			break;
		}
		case AS_LIST: {
			as_list* store_list = as_list_fromval(as_val_to_add);
			if (!store_list) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert list");
				return err->code;
			}
			as_record_set_list(record, bin, store_list);
			break;
		}
		case AS_MAP: {
			as_map* store_map = as_map_fromval(as_val_to_add);
			if (!store_map) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert map");
				return err->code;
			}
			as_record_set_map(record, bin, store_map);
			break;
		}
		case AS_GEOJSON: {
			as_geojson* store_geojson = as_geojson_fromval(as_val_to_add);
			if (!store_geojson) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to convert map");
				return err->code;
			}
			as_record_set_geojson(record, bin, store_geojson);
			break;
		}
		default:
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Unsupported type");
	}

	if (err->code != AEROSPIKE_OK) {
		return err->code;
	}
	return AEROSPIKE_OK;
}

//meta is a dict ["ttl"=>#, "gen"=>#]

as_status as_record_to_z_metadata(const as_record* record, zval* z_meta, as_error* err) {
	// This should never happen, but catch it if it does.
	if(Z_TYPE_P(z_meta) != IS_ARRAY) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "zval passed as metadata is not an array");
		return err->code;
	}

	add_assoc_long(z_meta, "ttl", record->ttl);
	add_assoc_long(z_meta, "generation", record->gen);
	return AEROSPIKE_OK;
}


/*
 * as_operate_record_to_zval
 *
 * Because operate returns a record with multiple entries per bin value, this function
 * iterates over each bin and adds an array of the form (bin_name, bin_value). These arrays
 * are stored in an array.
 * e.g. [['bin1', 5], ['bin1', 6], ['bin2', [3,4,5]]]
 * Other converter functions assume one entry per bin name.
 *
 * @param record the C client record to be converted to an array of arrays of bin values
 * @param z_bins Pointer to pointer to zval. It will be allocated and filled with list of tuples of bin values
 * @param err pointer to as_error. It will be filled if an error is encountered.
 */
as_status as_operate_record_to_zval(const as_record* record, zval* z_bins, as_error* err) {

	zval z_bin_pair;
	zval z_bin_value;
	as_record_iterator it;

	if (!record) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "NULL record in as_operate_record_to_zval");
		return err->code;
	}

	as_record_iterator_init(&it, record);
	array_init(z_bins);

	while (as_record_iterator_has_next(&it)) {
		as_bin* bin = as_record_iterator_next(&it);

		if (as_val_to_zval((as_val*)as_bin_get_value(bin), &z_bin_value, err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}

		array_init(&z_bin_pair);
		add_next_index_string(&z_bin_pair, bin->name);
		add_next_index_zval(&z_bin_pair, &z_bin_value);

		add_next_index_zval(z_bins, &z_bin_pair);
	}
CLEANUP:
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_bins);
		ZVAL_NULL(z_bins);
	}

	as_record_iterator_destroy(&it);
	return err->code;
}


/* Convert a PHP array into an as_key */
as_status z_hashtable_to_as_key(HashTable* z_key_hash, as_key* key, as_error* err) {
	zval* z_namespace = NULL;
	zval* z_set = NULL;
	zval* z_pk = NULL;

	bool is_digest = false;
	char* namespace_str = NULL;
	char* set_str = NULL;

	z_namespace = zend_hash_str_find(z_key_hash, NAMESPACE_KEY, strlen(NAMESPACE_KEY));
	if (!z_namespace || (Z_TYPE_P(z_namespace) != IS_STRING)) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Key must contain a string entry for ns");
		return err->code;
	}

	z_set = zend_hash_str_find(z_key_hash, SET_KEY, strlen(SET_KEY));

	// TODO No set is perfectly valid
	if (z_set && (Z_TYPE_P(z_set) != IS_STRING)) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Key entry for set must be a string");
		return err->code;
	}

	z_pk = zend_hash_str_find(z_key_hash, PK_KEY, strlen(PK_KEY));

	if (!z_pk) {
		z_pk = zend_hash_str_find(z_key_hash, DIGEST_KEY, strlen(DIGEST_KEY));
		if (z_pk && (Z_TYPE_P(z_pk) == IS_STRING)) {
			is_digest = true;
		}
		else {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Digest must be a string");
			return err->code;
		}
	}

	namespace_str = Z_STRVAL_P(z_namespace);
	// Only use set_str if an entry for "set" was provided
	if (!z_set) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Key must contain a string entry for set");
		return err->code;
	}
	set_str = Z_STRVAL_P(z_set);

	switch(Z_TYPE_P(z_pk)) {

		case IS_STRING: {

			// The user passed the digest directly, initialize the key
			if (is_digest) {
				if (Z_STRLEN_P(z_pk) != AS_DIGEST_VALUE_SIZE) {
					as_error_update(err, AEROSPIKE_ERR_PARAM, "Digest must be 20 bytes");
					return err->code;
				}
				if (as_key_init_digest(key, namespace_str, set_str, (uint8_t*)Z_STRVAL_P(z_pk))) {
					return AEROSPIKE_OK;
				}
			}
			// It was just a string, initialize key with the string
			else {
				if (as_key_init_strp(key, namespace_str, set_str, strdup(Z_STRVAL_P(z_pk)), true)) {
					return AEROSPIKE_OK;
				}
			}
			break;
		}

		case IS_LONG: {
			int64_t pk_int = (int64_t)Z_LVAL_P(z_pk);
			if (as_key_init_int64(key, namespace_str, set_str, pk_int)) {
				return AEROSPIKE_OK;
			}
			break;
		}
		default:
			return AEROSPIKE_ERR_CLIENT;
	}

	// Key creation failed, free the resources
	return AEROSPIKE_ERR_CLIENT;

}

/*

*/
/**
 * Function to check if a php value should map to an as_list, or an as_map.
 * We Define a list in php as a hashtable with only numeric keys, which start at 0, and are in ascending order
 * essentially:
 * i = 0;
 * for key in php_list {
 * 	if !is_int(key) or int(key) != i {
 * 		return false
 * 	}
 * 	i++
 * }
 *
 * @return true if the HashTable is an as_list, false indicates an as_map
 */
bool hashtable_is_list(HashTable* php_hash) {
	int i = 0;

	zend_ulong numeric_key;
	zend_string* string_key;

	ZEND_HASH_FOREACH_KEY(php_hash, numeric_key, string_key)
	{
		if (string_key) {
			return false;
		} else {
			if (i != numeric_key) {
				return false;
			}
			i++;
		}

	} ZEND_HASH_FOREACH_END();
	return true;
}

/**
  *Takes a php hashtable as an argument and stores it in the c_list
**/

/**
 * Convert a php array to an as_list
 *
 * @param php_hash			The php array to be converted to an as_list
 * @param list				the array list to be allocated and filled with data
 * @param err				The error structure to be filled on failure
 * @param serializer_type	The type of serializer to be used for storing non native data types
 *
 * @return	as_status indicating success/failure of the operation
 */
as_status z_hashtable_to_as_list(HashTable* php_hash, as_list** list, as_error* err, int serializer_type) {

	uint32_t size = zend_hash_num_elements(php_hash);

	if (*list == NULL) {
		*list = (as_list*)as_arraylist_new((uint32_t) size, 0);
	}

	if (*list == NULL) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to allocate memory for list");
		goto CLEANUP;
	}

	zval* php_value = NULL;
	ZEND_HASH_FOREACH_VAL(php_hash, php_value)
	{
		as_val* as_val_value = NULL;
		zval_to_as_val(php_value, &as_val_value, err, serializer_type);
		// Convert the php value into an as_value;
		if (!as_val_value || err->code != AEROSPIKE_OK) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to convert item to a list");
			goto CLEANUP;
		}

		as_list_append(*list, as_val_value);
	} ZEND_HASH_FOREACH_END();

CLEANUP:
	if (err->code != AEROSPIKE_OK) {
		if (*list) {
			as_list_destroy(*list);
		}
		*list = NULL;
	}
	return AEROSPIKE_OK;
}

/*
 * Takes a php hashtable and an allocated as_privileges** and fills it with as_privilege entries
 * returns err, and fills err param on failure
 *
 * This allocates individual privilege entries, so it is up to the caller to free them
 */
as_status z_hashtable_to_as_privileges(HashTable* z_priv_hash, as_privilege** privileges, as_error* err) {

	zval* z_privilege;
	HashTable* current_priv_hash;
	int i = 0;
	zval* privilege_entry = NULL;

	ZEND_HASH_FOREACH_VAL(z_priv_hash, z_privilege) {
		privileges[i] = (as_privilege*)malloc(1 * sizeof(as_privilege));
		if (Z_TYPE_P(z_privilege) != IS_ARRAY) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "privilege must be an array");
			goto CLEANUP;
		}
		current_priv_hash = Z_ARRVAL_P(z_privilege);

		/* Handle the code entry */
		privilege_entry = zend_hash_str_find(current_priv_hash, "code", strlen("code"));
		if (!privilege_entry) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "privilege must contain a code entry");
			goto CLEANUP;
		}
		if (Z_TYPE_P(privilege_entry) != IS_LONG) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "code must be an integer");
			goto CLEANUP;
		}
		privileges[i]->code = Z_LVAL_P(privilege_entry);

		/* Handle the ns entry */
		privilege_entry = zend_hash_str_find(current_priv_hash, "ns", strlen("ns"));
		if (!privilege_entry) {
			privileges[i]->ns[0] = '\0';
		} else {
			if (Z_TYPE_P(privilege_entry) != IS_STRING) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Namespace must be a string");
				goto CLEANUP;
			}
			if (Z_STRLEN_P(privilege_entry) >= AS_NAMESPACE_MAX_SIZE) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Namespace is too long");
				goto CLEANUP;
			}
			strcpy(privileges[i]->ns, Z_STRVAL_P(privilege_entry));
		}

		/* Handle the set entry */
		privilege_entry = zend_hash_str_find(current_priv_hash, "set", strlen("set"));
		if (!privilege_entry) {
			privileges[i]->set[0] = '\0';
		} else {
			if (Z_TYPE_P(privilege_entry) != IS_STRING) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Set must be a string");
				goto CLEANUP;
			}
			if (Z_STRLEN_P(privilege_entry) >= AS_SET_MAX_SIZE) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Set is too long");
				goto CLEANUP;
			}
			strcpy(privileges[i]->set, Z_STRVAL_P(privilege_entry));
		}

		i++;
	}ZEND_HASH_FOREACH_END();

CLEANUP:
	return err->code;
}

/*Convert a HashTable* into a char**. The char** must already have been allocated, roles size must
 * be the length of the array*/
as_status z_hash_to_str_array(HashTable* z_roles, char** roles, int max_size, int roles_size, as_error* err) {
	/* should never happen*/
	if (!roles) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "null array passed to z_hashtable_to_str_array");
		return AEROSPIKE_ERR_CLIENT;
	}

	int i = 0;
	zval* current_role = NULL;

	ZEND_HASH_FOREACH_VAL(z_roles, current_role) {

		if (i >= roles_size) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unexpected number of roles");
			return err->code;
		}
		/* verify that the current role is a valid string and copy to a char* */
		if (Z_TYPE_P(current_role) != IS_STRING) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Role must be a string");
			return err->code;
		}
		if (Z_STRLEN_P(current_role) >= max_size) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Role name is too long");
			return err->code;
		}

		roles[i] = strdup(Z_STRVAL_P(current_role));
		if (!roles[i]) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to allocate memory for role");
			return err->code;
		}

		i++;
	} ZEND_HASH_FOREACH_END();

	return err->code;
}
/*
 * Convert an array of as_role* into a zval array. This will setup z_roles and dtor it
 * in case of error.
 */
as_status as_roles_to_zval(as_role** roles, int roles_size, zval* z_roles, as_error* err) {
	zval current_role;
	ZVAL_NULL(&current_role);

	if (!roles && roles_size) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "NULL roles provided to roles_to_zval");
		return err->code;
	}

	array_init(z_roles);

	for (int i = 0; i < roles_size; i++) {
		if (as_role_to_zval(roles[i], &current_role, err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
		if (add_assoc_zval(z_roles, roles[i]->name, &current_role) == FAILURE) {
			zval_dtor(&current_role);
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add role to roles list");
		}
	}

CLEANUP:
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_roles);
		ZVAL_NULL(z_roles);
	}

	return err->code;

}

/* Convert an as_role into a php value
 * On error, this will cleanup the zval
 *
 */
as_status as_role_to_zval(as_role* role, zval* z_role, as_error* err) {
	zval current_privilege;
	ZVAL_NULL(&current_privilege);

	if (!role) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "NULL role");
		return AEROSPIKE_ERR_CLIENT;
	}
	array_init(z_role);

	for (int i = 0; i < role->privileges_size; i++) {
		if (as_privilege_to_zval(&role->privileges[i], &current_privilege, err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
		if (add_next_index_zval(z_role, &current_privilege) == FAILURE) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add privilege to role list");
			zval_dtor(&current_privilege);
			goto CLEANUP;
		}
	}

CLEANUP:
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_role);
		ZVAL_NULL(z_role);
	}
	return err->code;
}
/* Convert an as_privilege into a PHP value;
 * On error the err parameter will be filled and the error code returned.
 * In case of error the ref count of the zval param will be decremented by this function
*/
as_status as_privilege_to_zval(as_privilege* privilege, zval* z_privilege, as_error* err) {

	/* This should never happen */
	if (!privilege) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "NULL Privilege");
		return AEROSPIKE_ERR_CLIENT;
	}

	array_init(z_privilege);
	if (add_assoc_string(z_privilege, "ns", privilege->ns) == FAILURE) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to construct privilege");
		goto CLEANUP;
	}
	if (add_assoc_string(z_privilege, "set", privilege->set) == FAILURE) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to construct privilege");
		goto CLEANUP;
	}
	if (add_assoc_long(z_privilege, "code", privilege->code) == FAILURE) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to construct privilege");
		goto CLEANUP;
	}

CLEANUP:
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_privilege);
	}
	return err->code;
}

/**
 * Convert an as_key to a php array. The return looks like ["ns"=>"...", "set"=>"...", "key"=>"...", "digest"=>"..."]
 *
 * @param key		the as_key to convert
 * @param z_key		the zval to hold the key information. This must be an array before being passed.
 * @param show_pk	Whether the returned key should have an entry for "key"
 * @param err		Error structure filled by  function upon failure.
 *
 * @return as_status indicating success of operation
 */
as_status as_key_to_zval(const as_key* key, zval* z_key, bool show_pk, as_error* err) {

	if (!key) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Can not convert null key");
		return AEROSPIKE_ERR_CLIENT;
	}

	// THIS SHOULD NEVER HAPPEN
	if( !z_key) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "null zval passed to as_key_to_zval");
		return AEROSPIKE_ERR_CLIENT;
	}
	/* all validations have passed, intialize the php array */
	array_init(z_key);

	if(key->ns) {
		//They key has no namespace, so it is empty
		if (!strlen(key->ns)) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Key has no namespace");
			/* Since we made memory for this, we free the key array */
			zval_dtor(z_key);
			return AEROSPIKE_ERR_CLIENT;
		}
		add_assoc_string(z_key, "ns", (char*)key->ns);
	}

	if(key->set) {
		add_assoc_string(z_key, "set", (char*)key->set);
	}

	// PK May be string, int, bytes
	if (key->valuep && show_pk) {
		as_val * val = (as_val *) key->valuep;
		as_val_t type = as_val_type(val);
		
		switch(type) {
			case AS_STRING: {
				as_string* sval = as_string_fromval(val);
				add_assoc_string(z_key, "key", as_string_get(sval));
				break;
			}
			
			case AS_INTEGER: {
				as_integer* ival = as_integer_fromval(val);
				add_assoc_long(z_key, "key", as_integer_get(ival));
				break;
			}

			case AS_BYTES: {
				as_bytes* bval = as_bytes_fromval(val);
				if (bval) {
					uint32_t byte_size = as_bytes_size(bval);
					add_assoc_stringl(z_key, "key", (char*)as_bytes_get(bval), byte_size);
				}
				break;
			}
			default:
				break;

		}
	} else {
		add_assoc_null(z_key, "key");
	}


	if (key->digest.init) {
		add_assoc_stringl(z_key, "digest", (char*)key->digest.value, AS_DIGEST_VALUE_SIZE);
	}
	return AEROSPIKE_OK;
}


/**
 * Function to serialize a php zval that does not map to one of the as_val's natively.
 * If serializer_type is SERIALIZER_NONE, an error is set.
 * If serializer_type is SERIALIZER_USER: attempt to call a registered serialization function
 * If serializer_type is SERIALIZER_PHP: call the default PHP serializer.
 *
 * @param zval_to_convert		The PHP object to convert.
 * @param out_bytes				The as_bytes that will be allocated by the function
 * @param err					The error structure to be filled on failure
 * @param serializer_type		The type of serializer to attempt to use on the zval
 * 								one of SERIALIZER_NONE, SERIALIZER_USER, SERIALIZER_PHP
 * @return as_status indicating success or failure
 */
as_status serialize_php_object(zval* zval_to_convert, as_bytes** out_bytes, as_error* err, int serializer_type) {
	as_bytes* bytes = NULL;
	*out_bytes = NULL;
	php_serialize_data_t var_hash;
	smart_str buf = {0};

	if(serializer_type == SERIALIZER_USER) {
		serialize_with_user_function(zval_to_convert, &bytes, err, serializer_type);
		if (err->code != AEROSPIKE_OK) {
			if (bytes) {
				as_bytes_destroy(bytes);
			}
		} else {
			*out_bytes = bytes;
		}
		return err->code;

	} else if (serializer_type == SERIALIZER_PHP) {
		PHP_VAR_SERIALIZE_INIT(var_hash);
		php_var_serialize(&buf, zval_to_convert, &var_hash);
		PHP_VAR_SERIALIZE_DESTROY(var_hash);
		if (EG(exception) || !buf.s->val) {
			smart_str_free(&buf);
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to serialize object");
			return err->code;
		}
		else {
			bytes = as_bytes_new(buf.s->len);
			if (as_bytes_set(bytes, 0, (uint8_t*)buf.s->val, buf.s->len)) {
				as_bytes_set_type(bytes, AS_BYTES_PHP);
				smart_str_free(&buf);
				*out_bytes = bytes;
				return AEROSPIKE_OK;
			} else {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to serialize object");
				as_bytes_destroy(bytes);
				smart_str_free(&buf);
				return err->code;
			}
		}
	} else {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Serialization failed");
		return err->code;
	}

}

/**
 * Convert as_bytes to a zval
 * If the type is AS_BYTES_PHP, use the php deserializer
 * If the type is AS_BYTES_BLOB use the user deserializer if registered, else return a string
 * If the type is a different AS_BYTES_* error
 *
 * @param bytes				as_bytes to deserialize.
 * @param retval			zval to hold the deserialized object
 * @param err				error structure to be filled on failure
 *
 * @return as_status indicating success of operation
 */
as_status unserialize_as_bytes(const as_bytes* bytes, zval* retval, as_error* err) {
	const unsigned char *bytes_start_p = NULL;
	const unsigned char *bytes_end_p = NULL;

	if (as_bytes_get_type(bytes) == AS_BYTES_BLOB) {
		if (!AEROSPIKE_G(is_global_user_deserializer_registered)) {
			return as_bytes_to_zval_bytes(bytes, retval, err);
		} else {
			return unserialize_with_user_function(bytes, retval, err);
		}
	}
	if (as_bytes_get_type(bytes) != AS_BYTES_PHP) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unsupported bytes type");
		return AEROSPIKE_ERR_CLIENT;

		// Don't currently know how to deserialize other things
	}
	bytes_start_p = (const unsigned char*) bytes->value;
	bytes_end_p = bytes_start_p + bytes->size;
	php_unserialize_data_t var_hash;
	PHP_VAR_UNSERIALIZE_INIT(var_hash);

	if (!php_var_unserialize(retval, &bytes_start_p, bytes_end_p, &var_hash)) {
		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "PHP deserialization failed");
		return AEROSPIKE_ERR_CLIENT; // This failed to deserialize
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
	return AEROSPIKE_OK;
}

/*
 * Create an Aerospike\Bytes class and set class_instance->s = bytes value
 *
 */
as_status as_bytes_to_zval_bytes(const as_bytes* bytes, zval* retval, as_error* err) {
	zend_class_entry* ce = NULL;
	zend_string* class_name = zend_string_init(BYTES_CLASS_NAME, strlen(BYTES_CLASS_NAME), 0);

	if (!class_name) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to create bytes object, zend string");
		return err->code;
	}

	ce = zend_lookup_class(class_name);
	zend_string_release(class_name);

	if (!ce) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to find class Aerospike Bytes class");
		return err->code;
	}

	if (object_init_ex(retval, ce) == FAILURE) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to create Bytes object");
		return err->code;
	}

	zend_update_property_stringl(ce, retval, BYTES_STR_PROPERTY, BYTES_STR_PROPERTY_LEN, (char*)bytes->value, bytes->size);

	return err->code;

}

/*
 * Create an Aerospike\Bytes class and set class_instance->s = bytes value
 *
 */
as_status as_geojson_to_zval(as_geojson* geojson, zval* retval, as_error* err) {
	zval class_name;
	zval from_json_name;
	zval class_method_ary;

	array_init(&class_method_ary);

	zval from_json_args[1];

	ZVAL_STRINGL(&from_json_args[0], as_geojson_get(geojson), as_geojson_len(geojson));
	ZVAL_STRING(&class_name, GEOJSON_CLASS_NAME);
	ZVAL_STRING(&from_json_name, GEOJSON_STATIC_CONSTRUCTOR);

	add_next_index_zval(&class_method_ary, &class_name);
	add_next_index_zval(&class_method_ary, &from_json_name);

	/* Since we are calling a static method, function_name should be an array of form [class, method] */
	if (call_user_function(EG(function_table), NULL, &class_method_ary, retval, 1, from_json_args) == FAILURE) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to deserialize GeoJSON");
	}

	zval_dtor(&class_method_ary);
	zval_dtor(&from_json_args[0]);
	return err->code;

}
