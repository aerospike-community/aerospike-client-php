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

#include "aerospike_class.h"
#include "php_aerospike_types.h"
#include "conversions.h"
#include "policy_conversions.h"

#define DECLARE_LIST_OPERATION_VARS \
		as_key key;\
		bool key_initialized = false;\
		as_error err;\
		as_error_init(&err);\
		as_operations* operations = NULL;\
		bool operations_initialized = false;\
		as_policy_operate operate_policy;\
		as_policy_operate* operate_policy_p = NULL;\
		as_record* rec = NULL;\
		HashTable* z_key = NULL;\
		zval* z_operate_policy = NULL;\
		char* bin_name = NULL;\
		size_t bin_name_size;\
		AerospikeClient* php_client = NULL;\
		aerospike* as_client = NULL;

#define VALIDATE_CLIENT_AND_CONNECTION() \
	do {\
		if (!php_client || !php_client->is_valid || !php_client->as_client) { \
			update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid Aerospike object", false);\
			RETURN_LONG(AEROSPIKE_ERR_CLIENT);\
		}\
		if (!php_client->is_connected) {\
			update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Not connected to Aerospike server", false);\
			RETURN_LONG(AEROSPIKE_ERR_CLIENT);\
		}\
	}while(0)

#define CHECK_BIN_NAME() \
	do {\
		if (bin_name_size > AS_BIN_NAME_MAX_LEN) {\
			update_client_error(getThis(),AEROSPIKE_ERR_PARAM, "Bin name is too long", false);\
			RETURN_LONG(AEROSPIKE_ERR_PARAM);\
		}\
	}while(0)

static inline void cleanup_list_operation(as_key* key, bool key_initialized, as_operations* operations, bool operations_initialized, as_record* rec);
//static bool validate_list_operation_variables();
static inline bool setup_list_operation_variables(HashTable* z_key, as_key* key, as_error* err, zval* z_operate_policy,
		as_policy_operate* operate_policy, as_policy_operate** operate_policy_p, bool* key_initialized, as_operations** operations,
		bool* ops_initialized, aerospike* as);


/* {{{ proto int Aerospike::listSize( array key, string bin, int count [,array options ] )
    Count the elements of the list value in the bin */
PHP_METHOD(Aerospike, listSize) {
	DECLARE_LIST_OPERATION_VARS
	zval* return_count = NULL;
	as_error_init(&err);

	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();

	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsz/|z",
			&z_key, &bin_name, &bin_name_size, &return_count, &z_operate_policy) == FAILURE) {

		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listSize", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zval_dtor(return_count);
	ZVAL_NULL(return_count);
	// Set the return value to null
	CHECK_BIN_NAME();

	if(!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy,
			&operate_policy_p, &key_initialized, &operations, &operations_initialized,
			as_client)) {
		goto CLEANUP;
	}

	as_operations_add_list_size(operations, bin_name);

	as_status status = aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);
	if (status != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		goto CLEANUP;
	}
	ZVAL_LONG(return_count, as_record_get_int64(rec, bin_name, -1));

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listAppend( array key, string bin, mixed value [,array options ] )
    Add a single value (of any type) to the end of the list */
PHP_METHOD(Aerospike, listAppend) {
	DECLARE_LIST_OPERATION_VARS;
	zval* z_append_value = NULL;
	as_val* val_to_add = NULL;
	int serializer_type = INI_INT("aerospike.serializer");
	reset_client_error(getThis());
	as_error_init(&err);

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();

	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsz|z", &z_key, &bin_name, &bin_name_size, &z_append_value, &z_operate_policy)
			!= SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listAppend", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if(!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy,
			&operate_policy_p, &key_initialized, &operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid serializer value", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	zval_to_as_val(z_append_value, &val_to_add, &err, serializer_type);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		goto CLEANUP;
	}

	as_operations_add_list_append(operations, bin_name, val_to_add);

	as_status status = aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);
	if (status != AEROSPIKE_OK) {
		update_client_error(getThis(),err.code, err.message, err.in_doubt);
		goto CLEANUP;
	}

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listMerge( array key, string bin, mixed value [,array options ] )
    Add items to the end of a list */
PHP_METHOD(Aerospike, listMerge) {
	//public int listMerge ( array $key, string $bin, array $items [, array $options ] )
	DECLARE_LIST_OPERATION_VARS;
	HashTable* items = NULL;
	int serializer_type = INI_INT("aerospike.serializer");
	as_list* list_items = NULL;
	as_error_init(&err);

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	reset_client_error(getThis());

	as_client = php_client->as_client;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsh|z", &z_key, &bin_name, &bin_name_size,
			&items, & z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listSize", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if(!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy,
			&operate_policy_p, &key_initialized, &operations, &operations_initialized,
			as_client)) {
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid serializer value", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (!hashtable_is_list(items)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Items to merge must be a list", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if(z_hashtable_to_as_list(items, &list_items, &err, serializer_type) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, "Failed to create a list in listMerge", false);
		goto CLEANUP;
	}

	if (!as_operations_add_list_append_items(operations, bin_name, list_items)) {
		if (list_items) {
			as_list_destroy(list_items);
		}
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Failed to add operations", false);
		err.code = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listInsert( array key, string bin, int index, mixed value [,array options ] )
    Insert an element at the specified index of a list value in the bin */
PHP_METHOD(Aerospike, listInsert) {
	DECLARE_LIST_OPERATION_VARS;
	as_error_init(&err);
	zval* zval_to_add = NULL;
	as_val* val_to_add = NULL;
	zend_long index = 0;
	int serializer_type = INI_INT("aerospike.serializer");
	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "hslz|z",
			&z_key, &bin_name, &bin_name_size, &index, &zval_to_add, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listInsert", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if(!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy,
			&operate_policy_p, &key_initialized, &operations, &operations_initialized,
			as_client)) {
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid serializer value", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	zval_to_as_val(zval_to_add, &val_to_add, &err, serializer_type);

	if(!val_to_add ||err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Failed to convert value to add to list", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if	(!as_operations_add_list_insert(operations, bin_name, (int64_t)index, val_to_add)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Failed to add operations", false);
		as_val_destroy(val_to_add);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listInsertItems( array key, string bin, int index, array items [,array options ] )
    Insert items at the specified index of a list value in the bin */
PHP_METHOD(Aerospike, listInsertItems) {
	DECLARE_LIST_OPERATION_VARS;
	as_error_init(&err);
	as_list* values_to_add = NULL;
	HashTable* z_values_to_add = NULL;
	zend_long index = 0;
	int serializer_type = INI_INT("aerospike.serializer");
	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hslh|z",
			&z_key, &bin_name, &bin_name_size, &index, &z_values_to_add, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters for listInsertItems", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err,z_operate_policy, &operate_policy,
			&operate_policy_p, &key_initialized, &operations, &operations_initialized,
			as_client)) {
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid serializer value", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (!hashtable_is_list(z_values_to_add)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Values to add must be a list", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (z_hashtable_to_as_list(z_values_to_add, &values_to_add, &err, serializer_type) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (!as_operations_add_list_insert_items(operations, bin_name, (int64_t)index, values_to_add)) {
		if (values_to_add) {
			as_list_destroy(values_to_add);
		}
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", false);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listPop( array key, string bin, int index, mixed element [,array options ] )
    Remove and get back the list element at a given index of a list value in the bin */
PHP_METHOD(Aerospike, listPop) {
	DECLARE_LIST_OPERATION_VARS;
	as_error_init(&err);
	zval* retval = NULL;
	zend_long index = 0;
	int serializer_type = INI_INT("aerospike.serializer");
	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hslz/|z",
			&z_key, &bin_name, &bin_name_size, &index, &retval, &z_operate_policy) == FAILURE) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listPop", false);
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();
	// Remove any reference to the old value and zero the return parameter
	zval_dtor(retval);
	ZVAL_NULL(retval);

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy,
			&operate_policy_p, &key_initialized, &operations, &operations_initialized,
			as_client)) {
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid serializer value", false);
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (!as_operations_add_list_pop(operations, bin_name, (int64_t)index)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", false);
		goto CLEANUP;
	}

	as_status status = aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);
	if (status != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	as_val* popped_value = NULL;
	popped_value = (as_val*)as_record_get(rec, bin_name);
	if (!popped_value) {
		// This just means that nothing was popped, perfectly valid
		goto CLEANUP;
	}
	if (as_val_to_zval(popped_value, retval, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Failed to deserialize returned value", err.in_doubt);
	}

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listPopRange( array key, string bin, int index, int count,
 * array elements [,array options ] )
    Remove and get back list elements at a given index of a list value in the bin */
PHP_METHOD(Aerospike, listPopRange) {
	DECLARE_LIST_OPERATION_VARS;
	zend_long start_index;
	zend_long count;
	zval* retval = NULL;

	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsllz/|z",
			&z_key, &bin_name, &bin_name_size, &start_index, &count,
			&retval, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for listPopRange", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zval_dtor(retval);
	ZVAL_NULL(retval);

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if	(!as_operations_add_list_pop_range(operations, bin_name, (int64_t)start_index, (uint64_t)count)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", false);
		goto CLEANUP;
	}

	as_status status = aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);
	if (status != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	as_val* popped_value = NULL;
	popped_value = (as_val*)as_record_get(rec, bin_name);
	if (!popped_value) {
		// This just means that nothing was popped, perfectly valid
		goto CLEANUP;
	}

	if (as_val_to_zval(popped_value, retval, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Failed to deserialize returned value", err.in_doubt);
	}

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listRemove( array key, string bin, int index [,array options ] )
    Remove a list element at a given index of a list value in the bin */
PHP_METHOD(Aerospike, listRemove) {
	DECLARE_LIST_OPERATION_VARS;
	zend_long index;

	reset_client_error(getThis());

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsl|z",
			&z_key, &bin_name, &bin_name_size, &index, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for listRemove", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if(!as_operations_add_list_remove(operations, bin_name, (int64_t)index)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", false);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listRemoveRange( array key, string bin, int index, int count [,array options ] )
    Remove list elements at a given index of a list value in the bin */
PHP_METHOD(Aerospike, listRemoveRange) {
	DECLARE_LIST_OPERATION_VARS;
	zend_long index;
	zend_long count;

	reset_client_error(getThis());

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsll|z",
			&z_key, &bin_name, &bin_name_size, &index, &count, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for listRemoveRange", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if(!as_operations_add_list_remove_range(operations, bin_name, (int64_t)index, (uint64_t)count)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", err.in_doubt);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listTrim( array key, string bin, int index, int count [,array options ] )
    Removing all elements not in the range starting at a given index plus count */
PHP_METHOD(Aerospike, listTrim) {
	DECLARE_LIST_OPERATION_VARS;
	zend_long index;
	zend_long count;

	reset_client_error(getThis());

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsll|z",
			&z_key, &bin_name, &bin_name_size, &index, &count, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for listTrim", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if(!as_operations_add_list_trim(operations, bin_name, (int64_t)index, (uint64_t)count)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", err.in_doubt);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listClear( array key, string bin [,array options ] )
    Remove all the elements from the list value in the bin */
PHP_METHOD(Aerospike, listClear) {
	DECLARE_LIST_OPERATION_VARS;

	reset_client_error(getThis());

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hs|z",
			&z_key, &bin_name, &bin_name_size, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for listClear", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if(!as_operations_add_list_clear(operations, bin_name)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		update_client_error(getThis(), err.code, "Failed to add operations", err.in_doubt);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::listSet( array key, string bin, int index, mixed value [,array options ] )
    Set list element val at the specified index of a list value in the bin */
PHP_METHOD(Aerospike, listSet) {
	DECLARE_LIST_OPERATION_VARS;
	reset_client_error(getThis());
	zval* zval_to_add = NULL;
	as_val* val_to_add = NULL;
	zend_long index = 0;
	int serializer_type = INI_INT("aerospike.serializer");

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "hslz|z",
			&z_key, &bin_name, &bin_name_size, &index, &zval_to_add, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listSet", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();

	if(!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid serializer value");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	zval_to_as_val(zval_to_add, &val_to_add, &err, serializer_type);

	if(!val_to_add || err.code != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Failed to convert value to add to list");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if	(!as_operations_add_list_set(operations, bin_name, (int64_t)index, val_to_add)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Failed to add operations");
		as_val_destroy(val_to_add);
		goto CLEANUP;
	}

	aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listGet( array key, string bin, int index, mixed element [,array options ] )
    Get the list element at the specified index of a list value in the bin */
PHP_METHOD(Aerospike, listGet) {
	DECLARE_LIST_OPERATION_VARS;
	zval* retval = NULL;
	zend_long index;
	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hslz/|z",
			&z_key, &bin_name, &bin_name_size, &index, &retval, &z_operate_policy) == FAILURE) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to listGet", false);
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	CHECK_BIN_NAME();
	if(!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	zval_dtor(retval);
	ZVAL_NULL(retval);

	if (!as_operations_add_list_get(operations, bin_name, (int64_t)index)) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Failed to add operations");
		goto CLEANUP;
	}

	if (aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	as_val* returned_value = NULL;
	returned_value = (as_val*)as_record_get(rec, bin_name);
	if (!returned_value) {
		// This just means that nothing was popped, perfectly valid
		goto CLEANUP;
	}
	if (as_val_to_zval(returned_value, retval, &err) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Failed to deserialize returned value");
	}

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listGetRange( array key, string bin, int index, int count,
 * array elements [,array options ] )
    Get the list of $count elements starting at a specified index of a list value in the bin */
PHP_METHOD(Aerospike, listGetRange) {
	DECLARE_LIST_OPERATION_VARS;
	zend_long start_index;
	zend_long count;
	zval* retval = NULL;

	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	VALIDATE_CLIENT_AND_CONNECTION();
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hsllz/|z",
			&z_key, &bin_name, &bin_name_size, &start_index, &count,
			&retval, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for listGetRange", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zval_dtor(retval);
	ZVAL_NULL(retval);

	CHECK_BIN_NAME();

	if (!setup_list_operation_variables(z_key, &key, &err, z_operate_policy, &operate_policy, &operate_policy_p, &key_initialized,
			&operations, &operations_initialized, as_client)) {
		goto CLEANUP;
	}

	if	(!as_operations_add_list_get_range(operations, bin_name, (int64_t)start_index, (uint64_t)count)) {
		err.code = AEROSPIKE_ERR_CLIENT;
		as_error_update(&err, err.code, "Failed to add operations");
		goto CLEANUP;
	}

	as_status status = aerospike_key_operate(as_client, &err, operate_policy_p, &key, operations, &rec);
	if (status != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	as_val* returned_value = NULL;
	returned_value = (as_val*)as_record_get(rec, bin_name);
	if (!returned_value) {
		/* This just means that nothing was popped, perfectly valid
		*  but this function always returns an array so set that up */
		array_init(retval);
		goto CLEANUP;
	}

	if (as_val_to_zval(returned_value, retval, &err) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Failed to deserialize returned value");
	}

CLEANUP:
	cleanup_list_operation(&key, key_initialized, operations, operations_initialized, rec);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* Initialize all variables needed for list operations, also validate the key,
 * policy, and generation. Return true on success, false on failure.
 */
static inline bool setup_list_operation_variables(HashTable* z_key, as_key* key,
		as_error* err, zval* z_operate_policy, as_policy_operate* operate_policy,
		as_policy_operate** operate_policy_p, bool* key_initialized,
		as_operations** operations, bool* operations_initialized, aerospike* as) {

	as_error_init(err);
	if (z_hashtable_to_as_key(z_key, key, err) != AEROSPIKE_OK) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid key");
		err->code = AEROSPIKE_ERR_PARAM;
		return false;
	}
	*key_initialized = true;

	if (AEROSPIKE_OK == zval_to_as_policy_operate(z_operate_policy, operate_policy,
			operate_policy_p, &as->config.policies.operate)) {
		*operate_policy_p = operate_policy;
	} else {
		err->code = AEROSPIKE_ERR_PARAM;
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid operate policy");
		return false;
	}
	*operations = as_operations_new(1);
	*operations_initialized = true;

	if (set_operations_generation_from_operate_policy(*operations, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		err->code = AEROSPIKE_ERR_PARAM;
		return false;
	}

	if (set_operations_ttl_from_operate_policy(*operations, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid TTL");
		err->code = AEROSPIKE_ERR_PARAM;
		return false;
	}

	return true;
}


/* De-allocate any memory created by the list operation */
static inline void cleanup_list_operation(as_key* key, bool key_initialized, as_operations* operations,
		bool operations_initialized, as_record* rec) {

	if (key_initialized) {
		as_key_destroy(key);
	}
	if (operations_initialized) {
		as_operations_destroy(operations);
	}
	if (rec) {
		as_record_destroy(rec);
	}
}
