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

static inline bool op_requires_long_val(int op_type);
static inline bool op_requires_list_val(int op_type);
static inline bool op_requires_val(int op_type);
static inline bool op_requires_as_val(int op_type);
static inline bool op_requires_index(int op_type);
//static inline bool requires_range(op);
//static inline bool requires_index(op);

static as_status add_op_to_operations(HashTable* op_array, as_operations* ops, as_error* err, int serializer_type);


/* {{{ proto int Aerospike::operate( array key, array operations [,array &returned [,array options ]] )
    Performs multiple operation on a record */
PHP_METHOD(Aerospike, operate) {
	as_error err;
	as_operations ops;
	as_policy_operate operate_policy;
	as_policy_operate* operate_policy_p = NULL;
	zval* z_operate_policy = NULL;
	HashTable* z_ops = NULL;
	HashTable* z_key = NULL;
	as_key key;
	zval* retval = NULL;
	aerospike* as_client = NULL;
	AerospikeClient* php_client = NULL;
	int operations_size = 0;
	bool key_initialized = false;
	bool operations_initialized = false;
	as_record* rec = NULL;
	int serializer_type = INI_INT("aerospike.serializer");

	as_error_init(&err);
	reset_client_error(getThis());

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "hh|z/z", &z_key, &z_ops, &retval, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for operate");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	if (retval) {
		zval_dtor(retval);
		ZVAL_NULL(retval);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (!php_client || !php_client->as_client || !php_client->is_valid) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid aerospike object");
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	as_client = php_client->as_client;

	if (!php_client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLUSTER, "No connection to Aerospike server");
		RETURN_LONG(AEROSPIKE_ERR_CLUSTER);
	}

	if(z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	if(zval_to_as_policy_operate(z_operate_policy, &operate_policy,
			&operate_policy_p, &as_client->config.policies.operate) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid operate policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	} else {
		operate_policy_p = &operate_policy;
	}

	operations_size = zend_hash_num_elements(z_ops);
	if (!operations_size) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Empty operations array");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}
	set_serializer_from_policy_hash(&serializer_type, z_operate_policy);

	as_operations_inita(&ops, operations_size);
	operations_initialized = true;

	if (set_operations_generation_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (set_operations_ttl_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (!hashtable_is_list(z_ops)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Operations array must be a list");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	zval* current_op = NULL;

	ZEND_HASH_FOREACH_VAL(z_ops, current_op)
	{

		if (Z_TYPE_P(current_op) != IS_ARRAY) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Operation must be an array");
			err.code = AEROSPIKE_ERR_PARAM;
			goto CLEANUP;
		}

		if (add_op_to_operations(Z_ARRVAL_P(current_op), &ops, &err, serializer_type) != AEROSPIKE_OK) {
			update_client_error(getThis(), err.code, err.message);
			goto CLEANUP;
		}

	}ZEND_HASH_FOREACH_END();

	if (aerospike_key_operate(as_client, &err, operate_policy_p, &key, &ops, &rec) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
		goto CLEANUP;
	}

	if (retval) {
		as_bins_to_zval(rec, retval, &err);
	}

CLEANUP:
	if (key_initialized) {
		as_key_destroy(&key);
	}
	if (operations_initialized) {
		as_operations_destroy(&ops);
	}
	if (rec) {
		as_record_destroy(rec);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::operateOrdered( array key, array operations [,array &returned [,array options ]] )
   Performs multiple operation on a record */
PHP_METHOD(Aerospike, operateOrdered) {
	as_error err;
	as_operations ops;
	as_policy_operate operate_policy;
	as_policy_operate* operate_policy_p = NULL;
	zval* z_operate_policy = NULL;
	HashTable* z_ops = NULL;
	HashTable* z_key = NULL;
	as_key key;
	zval* retval = NULL;
	aerospike* as_client = NULL;
	AerospikeClient* php_client = NULL;
	int operations_size = 0;
	bool key_initialized = false;
	bool operations_initialized = false;
	as_record* rec = NULL;
	int serializer_type = INI_INT("aerospike.serializer");

	as_error_init(&err);
	reset_client_error(getThis());

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "hh|z/z", &z_key, &z_ops, &retval, &z_operate_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for operateOrdered");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	if (retval) {
		zval_dtor(retval);
		ZVAL_NULL(retval);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (!php_client || !php_client->as_client || !php_client->is_valid) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid aerospike object");
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	as_client = php_client->as_client;

	if (!php_client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLUSTER, "No connection to Aerospike server");
		RETURN_LONG(AEROSPIKE_ERR_CLUSTER);
	}

	if(z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	if (z_operate_policy){
		if(zval_to_as_policy_operate(z_operate_policy, &operate_policy,
				&operate_policy_p, &as_client->config.policies.operate) != AEROSPIKE_OK) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid operate policy");
			err.code = AEROSPIKE_ERR_PARAM;
			goto CLEANUP;
		}
	}

	operations_size = zend_hash_num_elements(z_ops);
	if (!operations_size) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Empty operations array");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}
	set_serializer_from_policy_hash(&serializer_type, z_operate_policy);

	as_operations_inita(&ops, operations_size);
	operations_initialized = true;

	if (set_operations_generation_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}


	if (set_operations_ttl_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid TTL");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	zval* current_op = NULL;
	zend_string* string_key = NULL;

	ZEND_HASH_FOREACH_STR_KEY_VAL(z_ops, string_key, current_op)
	{
		if (string_key) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Operations array must not have string keys");
			err.code = AEROSPIKE_ERR_PARAM;
			goto CLEANUP;
		}

		if (Z_TYPE_P(current_op) != IS_ARRAY) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Operation must be an array");
			err.code = AEROSPIKE_ERR_PARAM;
			goto CLEANUP;
		}

		if (add_op_to_operations(Z_ARRVAL_P(current_op), &ops, &err, serializer_type) != AEROSPIKE_OK) {
			update_client_error(getThis(), err.code, err.message);
			goto CLEANUP;
		}

	}ZEND_HASH_FOREACH_END();

	if (aerospike_key_operate(as_client, &err, operate_policy_p, &key, &ops, &rec) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
		goto CLEANUP;
	}


	if (retval) {
		as_operate_record_to_zval(rec, retval, &err);
	}

CLEANUP:
	if (key_initialized) {
		as_key_destroy(&key);
	}
	if (operations_initialized) {
		as_operations_destroy(&ops);
	}
	if (rec) {
		as_record_destroy(rec);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* Helper function to add a php operation array to an as_operations struct
 * Internally checks and validates entries in the array, then adds the operation to
 * the operations.
 * returns AEROSPIKE_OK on success, other status code on failure
 */
static as_status add_op_to_operations(HashTable* op_array, as_operations* ops, as_error* err, int serializer_type) {
	int op_type;
	long index;
	zval* z_op = NULL;
	zval* z_op_val = NULL;
	zval* z_index = NULL;
	zval* z_bin = NULL;
	as_val* op_val = NULL;
	char* bin_name = NULL;

	z_op = zend_hash_str_find(op_array, "op", strlen("op"));

	if (!z_op) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation does not specify a type");
		return AEROSPIKE_ERR_PARAM;
	}
	if (Z_TYPE_P(z_op) != IS_LONG) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation type must be an integer");
		return AEROSPIKE_ERR_PARAM;
	}
	op_type = Z_LVAL_P(z_op);

	//handle touch differently since it is unique
	if (op_type == AS_OPERATOR_TOUCH) {
		z_op_val = zend_hash_str_find(op_array, "ttl", strlen("ttl"));
		if (z_op_val) {
			if (Z_TYPE_P(z_op_val) != IS_LONG) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "TTL must be an integer");
				return AEROSPIKE_ERR_PARAM;
			}
			ops->ttl = (uint32_t)Z_LVAL_P(z_op_val);
			as_operations_add_touch(ops);
		}

		return AEROSPIKE_OK;
	}
	z_bin = zend_hash_str_find(op_array, "bin", strlen("bin"));
	if (!z_bin || Z_TYPE_P(z_bin) != IS_STRING) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must contain a bin name of string type");
		return AEROSPIKE_ERR_PARAM;
	}
	bin_name = Z_STRVAL_P(z_bin);

	if (strlen(bin_name) > AS_BIN_NAME_MAX_LEN) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Bin name is too long");
		return AEROSPIKE_ERR_PARAM;
	}

	/*
	 * Verify required types and arguments for each operation
	 */
	if (op_requires_index(op_type)) {
		z_index = zend_hash_str_find(op_array, "index", strlen("index"));
		if (!z_index || Z_TYPE_P(z_index) != IS_LONG) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation requires an integer index");
			return AEROSPIKE_ERR_PARAM;
		}
		index = Z_LVAL_P(z_index);
	}
	/*
	 * Type check "val" member of the op array
	 */
	if (op_requires_val(op_type)) {
		z_op_val = zend_hash_str_find(op_array, "val", strlen("val"));
		if (!z_op_val) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation requires a val entry");
			return AEROSPIKE_ERR_PARAM;
		}

		if (op_requires_list_val(op_type)) {
			if (Z_TYPE_P(z_op_val) != IS_ARRAY) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation requires a list val entry");
				return AEROSPIKE_ERR_PARAM;
			}
		} else if (op_requires_long_val(op_type)) {
			if (Z_TYPE_P(z_op_val) != IS_LONG) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation requires an integer val entry");
				return AEROSPIKE_ERR_PARAM;
			}
		}
		if (op_requires_as_val(op_type)) {
			zval_to_as_val(z_op_val, &op_val, err, serializer_type);
			if (!op_val || err->code != AEROSPIKE_OK) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Unable to convert value");
				return AEROSPIKE_ERR_PARAM;
			}
		}
	}

	/*
	 * Handle each different operation type differently
	 */
	switch(op_type) {
		case AS_OPERATOR_WRITE: {
			if (!as_operations_add_write(ops, bin_name, (as_bin_value*)op_val)) {
				as_val_destroy(op_val);
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case AS_OPERATOR_READ: {
			if (!as_operations_add_read(ops, bin_name)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case AS_OPERATOR_INCR: {
			if (Z_TYPE_P(z_op_val) == IS_LONG) {
				if (!as_operations_add_incr(ops, bin_name, Z_LVAL_P(z_op_val))) {
					as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
					return AEROSPIKE_ERR_CLIENT;
				}
			} else if (Z_TYPE_P(z_op_val) == IS_DOUBLE) {
				if (!as_operations_add_incr_double(ops, bin_name, Z_DVAL_P(z_op_val))) {
					as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
					return AEROSPIKE_ERR_CLIENT;
				}
			} else if (Z_TYPE_P(z_op_val) == IS_STRING) {
				zend_long incr_long;
				double incr_double;
				switch (is_numeric_string(Z_STRVAL_P(z_op_val), Z_STRLEN_P(z_op_val),
						&incr_long, &incr_double, 0)) {
					case IS_DOUBLE:
						if (!as_operations_add_incr_double(ops, bin_name, incr_double)) {
							as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
							return AEROSPIKE_ERR_CLIENT;
						}
						break;
					case IS_LONG:
						if (!as_operations_add_incr(ops, bin_name, incr_long)) {
							as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
							return AEROSPIKE_ERR_CLIENT;
						}
						break;
					default:
						as_error_update(err, AEROSPIKE_ERR_PARAM, "incr operation requires a long or double value");
						return AEROSPIKE_ERR_PARAM;
				}
			} else {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "incr operation requires a long or double value");
				return AEROSPIKE_ERR_PARAM;
			}
			break;
		}
		case AS_OPERATOR_PREPEND: {
			if (Z_TYPE_P(z_op_val) != IS_STRING) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Prepend requires a string value");
				return AEROSPIKE_ERR_PARAM;
			}
			if (!as_operations_add_prepend_str(ops, bin_name, Z_STRVAL_P(z_op_val))) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case AS_OPERATOR_APPEND: {
			if (Z_TYPE_P(z_op_val) != IS_STRING) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Append requires a string value");
				return AEROSPIKE_ERR_PARAM;
			}
			if (!as_operations_add_append_str(ops, bin_name, Z_STRVAL_P(z_op_val))) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		/** Start of list operations **/
		case OP_LIST_APPEND: {
			if (!as_operations_add_list_append(ops, bin_name, op_val)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_MERGE: {
			/* This op requires a zval list*/
			// We know this is a list because of the check for non list earlier
			if (!hashtable_is_list(Z_ARRVAL_P(z_op_val))) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "List Merge value must not be a map");
				return AEROSPIKE_ERR_CLIENT;
			}
			as_list* op_list = NULL;
			if (z_hashtable_to_as_list(
					Z_ARRVAL_P(z_op_val), &op_list, err, serializer_type) != AEROSPIKE_OK) {
				return err->code;
			}

			if (!as_operations_add_list_append_items(ops, bin_name, op_list)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_INSERT: {
			if(!as_operations_add_list_insert(ops, bin_name, index, op_val)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_INSERT_ITEMS: {
			if (!hashtable_is_list(Z_ARRVAL_P(z_op_val))) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "List Merge value must not be a map");
				return AEROSPIKE_ERR_CLIENT;
			}
			as_list* op_list = NULL;
			if (z_hashtable_to_as_list(
					Z_ARRVAL_P(z_op_val), &op_list, err, serializer_type) != AEROSPIKE_OK) {
				return err->code;
			}

			if (!as_operations_add_list_insert_items(ops, bin_name, index, (as_list*)op_list)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_POP: {
			if(!as_operations_add_list_pop(ops, bin_name, index)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_POP_RANGE: {
			long count = Z_LVAL_P(z_op_val);
			if(!as_operations_add_list_pop_range(ops, bin_name, index, count)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_REMOVE: {
			if(!as_operations_add_list_remove(ops, bin_name, index)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_REMOVE_RANGE: {
			long count = Z_LVAL_P(z_op_val);
			if(!as_operations_add_list_remove_range(ops, bin_name, index, count)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_CLEAR: {
			if(!as_operations_add_list_clear(ops, bin_name)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_SET: {
			if(!as_operations_add_list_set(ops, bin_name, index, op_val)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_GET: {
			if(!as_operations_add_list_get(ops, bin_name, index)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_GET_RANGE: {
			long count = Z_LVAL_P(z_op_val);
			if(!as_operations_add_list_get_range(ops, bin_name, index, count)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_TRIM: {
			long count = Z_LVAL_P(z_op_val);
			if(!as_operations_add_list_trim(ops, bin_name, index, count)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		case OP_LIST_SIZE: {
			if(!as_operations_add_list_size(ops, bin_name)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add operation");
				return AEROSPIKE_ERR_CLIENT;
			}
			break;
		}
		default: {
			if (op_val) {
				as_val_destroy(op_val);
			}
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Unknown operation type");
			return AEROSPIKE_ERR_PARAM;
		}
	}

	return AEROSPIKE_OK;
}

static inline bool op_requires_val(int op_type) {
	return (op_type != AS_OPERATOR_TOUCH && op_type != OP_LIST_CLEAR  &&
			op_type != AS_OPERATOR_READ  && op_type != OP_LIST_SIZE   &&
			op_type != OP_LIST_POP       && op_type != OP_LIST_REMOVE &&
			op_type != OP_LIST_GET);
}

static inline bool op_requires_list_val(int op_type) {
	return (op_type == OP_LIST_MERGE || op_type == OP_LIST_INSERT_ITEMS);
}

static inline bool op_requires_long_val(int op_type) {
	return (op_type == OP_LIST_POP_RANGE || op_type == OP_LIST_REMOVE_RANGE ||
			op_type == OP_LIST_GET_RANGE || op_type == OP_LIST_TRIM);
}

static inline bool op_requires_as_val(int op_type) {
	return (op_type == OP_LIST_INSERT    || op_type == OP_LIST_SET ||
			op_type == OP_LIST_APPEND    || op_type == AS_OPERATOR_WRITE);
}

static inline bool op_requires_index(int op_type) {
	return (op_type == OP_LIST_INSERT    || op_type == OP_LIST_INSERT_ITEMS ||
			op_type == OP_LIST_POP       || op_type == OP_LIST_POP_RANGE    ||
			op_type == OP_LIST_REMOVE    || op_type == OP_LIST_REMOVE_RANGE ||
			op_type == OP_LIST_SET       || op_type == OP_LIST_GET          ||
			op_type == OP_LIST_GET_RANGE || op_type == OP_LIST_TRIM);
}
