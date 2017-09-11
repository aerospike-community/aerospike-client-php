// *****************************************************************************
// Copyright 2017 Aerospike, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// *****************************************************************************

#include "php.h"
#include "aerospike_class.h"
#include "php_aerospike_types.h"
#include "aerospike/as_status.h"
#include "aerospike/as_bin.h"

as_status geo_val_to_json(zval* geo_data, zval* retval);


/* {{{ proto array Aerospike::predicateEquals( string bin, int|string val )
    Helper which builds the 'WHERE EQUALS' predicate */
PHP_METHOD(Aerospike, predicateEquals)
{
	char* bin = NULL;
	size_t bin_len = 0;
	zval* value = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz", &bin, &bin_len, &value) != SUCCESS) {
		RETURN_NULL();
	}

	// Return null if the bin name is too long
	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	// The value must be a string or integer, return null if neither
	if (Z_TYPE_P(value) != IS_STRING && Z_TYPE_P(value) != IS_LONG) {
		RETURN_NULL();
	}
	// The value must be a string or integer, return null if neither
	array_init(return_value);
	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_EQUAL);
	Z_TRY_ADDREF_P(value);
	add_assoc_zval(return_value, "val", value);

	return;
}
/* }}} */


/* {{{ proto array Aerospike::predicateBetween( string bin, int min, int max )
    Helper which builds the 'WHERE BETWEEN' predicate */
PHP_METHOD(Aerospike, predicateBetween) {
	char* bin = NULL;
	size_t bin_len;
	zend_long min;
	zend_long max;
	zval min_max_ary;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sll", &bin, &bin_len, &min, &max) != SUCCESS) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	array_init(&min_max_ary);
	add_next_index_long(&min_max_ary, min);
	add_next_index_long(&min_max_ary, max);

	array_init(return_value);
	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_BETWEEN);
	add_assoc_zval(return_value, "val", &min_max_ary);

	return;
}
/* }}} */


/* {{{ proto array Aerospike::predicateRange( string bin, int index_type,
 * int min, int max )
 Helper which builds the 'WHERE RANGE' predicate */
PHP_METHOD(Aerospike, predicateRange) {
	char* bin = NULL;
	size_t bin_len = 0;
	zend_long index_type;
	zend_long min;
	zend_long max;
	zval min_max_ary;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "slll",
			&bin, &bin_len, &index_type, &min, &max) != SUCCESS) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	array_init(&min_max_ary);
	add_next_index_long(&min_max_ary, min);
	add_next_index_long(&min_max_ary, max);

	array_init(return_value);
	add_assoc_string(return_value, "bin", bin);
	add_assoc_long(return_value, "index_type", index_type);
	add_assoc_string(return_value, "op", PHP_PREDICATE_RANGE);
	add_assoc_zval(return_value, "val", &min_max_ary);

	return;
}
/* }}} */


/* {{{ proto array Aerospike::predicateContains( string bin, int index_type, int|string val )
    Helper which builds the 'WHERE CONTAINS' predicate */
PHP_METHOD(Aerospike, predicateContains) {

	char* bin = NULL;
	size_t bin_len = 0;
	zval* value = NULL;
	zend_long index_type;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "slz", &bin, &bin_len, &index_type, &value) != SUCCESS) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	if (Z_TYPE_P(value) != IS_STRING && Z_TYPE_P(value) != IS_LONG) {
		RETURN_NULL();
	}

	array_init(return_value);
	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_CONTAINS);
	add_assoc_long(return_value, "index_type", index_type);
	Z_TRY_ADDREF_P(value);
	add_assoc_zval(return_value, "val", value);

	return;
}
/* }}} */


/* {{{proto array Aerospike::predicateGeoContainsGeoJSONPoint(string bin, string
 * Point).
 * Helper which build the "CONRAINS POINT" predicate */
PHP_METHOD(Aerospike, predicateGeoContainsGeoJSONPoint) {
	char* json_point = NULL;
	size_t json_length;
	char* bin = NULL;
	size_t bin_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &bin, &bin_len,
			&json_point, &json_length) != SUCCESS) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	array_init(return_value);
	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_GEO_CONTAINS);
	add_assoc_stringl(return_value, "val", json_point, json_length);

}
/* }}} */


/* {{{ proto array Aerospike::predicateGeoWithinGeoJSONRegion( string bin,
 * string region).
 * Helper which builds the "WHITHIN REGION' predicate */
PHP_METHOD(Aerospike, predicateGeoWithinGeoJSONRegion) {
	char* json_region = NULL;
	size_t json_region_len;

	char* bin = NULL;
	size_t bin_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss",
			&bin, &bin_len,
			&json_region, &json_region_len) == FAILURE) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	array_init(return_value);
	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_GEO_WITHIN);
	add_assoc_stringl(return_value, "val", json_region, json_region_len);

}
/* }}} */


/* {{{proto array Aerospike::predicateGeoContainsPoint(string bin, double long,
 * double lat).
 * Helper which build the "CONTAINS POINT" predicate */
PHP_METHOD(Aerospike, predicateGeoContainsPoint) {
	char* bin = NULL;
	double lat;
	double longitude;
	zval lat_long_pair;
	zval geo_array;
	size_t bin_len;
	zval json_str;

	ZVAL_NULL(&geo_array);
	ZVAL_NULL(&json_str);
	ZVAL_NULL(&lat_long_pair);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sdd", &bin, &bin_len,
			&lat, &longitude) != SUCCESS) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	array_init(&lat_long_pair);
	add_next_index_double(&lat_long_pair, lat);
	add_next_index_double(&lat_long_pair, longitude);

	array_init(&geo_array);
	add_assoc_string(&geo_array, "type", "Point");
	add_assoc_zval(&geo_array, "coordinates", &lat_long_pair);

	as_status status = geo_val_to_json(&geo_array, &json_str);
	zval_dtor(&geo_array);

	if (status != AEROSPIKE_OK) {
		RETURN_NULL();
	}
	array_init(return_value);

	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_GEO_CONTAINS);
	add_assoc_stringl(return_value, "val", Z_STRVAL(json_str), Z_STRLEN(json_str));

	zval_dtor(&json_str);

}
/* }}} */


/* {{{proto array Aerospike::predicateGeoWithinRadius( string bin, double long, double $lat,
 * float $radiusMeter).
 * Helper which builds the "WITHIN RADIUS" predicate */
PHP_METHOD(Aerospike, predicateGeoWithinRadius) {
	char* bin = NULL;
	double lat;
	double longitude;
	double radius;

	size_t bin_len;

	zval lat_long_pair;
	zval json_str;
	zval coordinates;
	zval geo_array;

	ZVAL_NULL(&geo_array);
	ZVAL_NULL(&json_str);
	ZVAL_NULL(&lat_long_pair);
	ZVAL_NULL(&coordinates);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sddd", &bin, &bin_len,
			&lat, &longitude, &radius) != SUCCESS) {
		RETURN_NULL();
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		RETURN_NULL();
	}

	/* create the [1.5, 1.5] lat, long array */
	array_init(&lat_long_pair);
	add_next_index_double(&lat_long_pair, lat);
	add_next_index_double(&lat_long_pair, longitude);

	/* Create the [ [1.5, 1.5], 2.5] coordinates array */
	array_init(&coordinates);
	add_next_index_zval(&coordinates, &lat_long_pair);
	add_next_index_double(&coordinates, radius);

	/* create the ["type"=>"AeroCircle", "coordinates"=>coordinates] */
	array_init(&geo_array);
	add_assoc_string(&geo_array, "type", "AeroCircle");
	add_assoc_zval(&geo_array, "coordinates", &coordinates);

	as_status status = geo_val_to_json(&geo_array, &json_str);
	zval_dtor(&geo_array);

	if (status != AEROSPIKE_OK) {
		RETURN_NULL();
	}
	array_init(return_value);

	add_assoc_string(return_value, "bin", bin);
	add_assoc_string(return_value, "op", PHP_PREDICATE_GEO_CONTAINS);
	add_assoc_stringl(return_value, "val", Z_STRVAL(json_str), Z_STRLEN(json_str));

	zval_dtor(&json_str);

}
/* }}} */


/* Helper function to call json_encode on a user land argument passed to one of the geo predicate functions */
as_status geo_val_to_json(zval* geo_data, zval* retval) {
	zval params[1];
	zval function_name;

	ZVAL_STRING(&function_name, "json_encode");
	params[0] = *geo_data;

	if (call_user_function(EG(function_table), NULL, &function_name, retval, 1, params) == FAILURE) {
		zval_dtor(&function_name);
		return AEROSPIKE_ERR_PARAM;
	}

	zval_dtor(&function_name);
	return AEROSPIKE_OK;
}
