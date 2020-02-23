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
static inline bool op_requires_bin(int op_type);
static inline bool op_requires_val(int op_type);
static inline bool op_requires_as_val(int op_type);
static inline bool op_requires_index(int op_type);
static inline bool op_is_map_op(int op_type);

/* Map op helpers */
static inline bool map_op_requires_key(int op_type);
static inline bool map_op_requires_rank(int op_type);
static inline bool map_op_requires_return_type(int op_type);
static inline bool map_op_requires_map_policy(int op_type);
static inline bool map_op_requires_val(int op_type);
static inline bool map_op_requires_index(int op_type);
static inline bool map_op_requires_count(int op_type);
static inline bool map_op_requires_range_end(int op_type);

static as_status add_map_op_to_operations(HashTable* op_array, int op_type, const char* bin_name,
		as_operations* ops, as_error* err, int serializer_type);
static as_status add_op_to_operations(HashTable* op_array, as_operations* ops, as_error* err, int serializer_type);

static as_status get_count_from_op_hash(as_error* err, HashTable* op_hash, uint64_t* count);
static as_status get_rank_from_op_hash(as_error* err, HashTable* op_hash, int64_t* rank);
static as_status get_index_from_op_hash(as_error* err, HashTable* op_hash, int64_t* index);
static as_status get_key_from_op_hash(as_error* err, HashTable* op_hash, as_val** key, int serializer_type);
static as_status get_value_from_op_hash(as_error* err, HashTable* op_hash, as_val** val, int serializer_type);
static as_status get_range_end_from_op_hash(as_error* err, HashTable* op_hash, as_val** range_end, int serializer_type);
static as_status get_map_policy_from_op_hash(as_error* err, HashTable* op_hash, as_map_policy* map_policy_p);
static as_status get_return_type_from_op_hash(as_error* err, HashTable* op_hash, as_map_return_type* return_type);

#define AS_MAP_POLICY_KEY "map_policy"
#define AS_MAP_RANK_KEY "rank"
#define AS_MAP_COUNT_KEY "count"
#define AS_MAP_KEY_KEY "key"
#define AS_MAP_INDEX_KEY "index"
#define AS_MAP_RETURN_TYPE_KEY "return_type"
#define AS_MAP_VALUE_KEY "val"
#define AS_MAP_RANGE_END "range_end"


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
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for operate", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	if (retval) {
		zval_dtor(retval);
		ZVAL_NULL(retval);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (!php_client || !php_client->as_client || !php_client->is_valid) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid aerospike object", false);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	as_client = php_client->as_client;

	if (!php_client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLUSTER, "No connection to Aerospike server", false);
		RETURN_LONG(AEROSPIKE_ERR_CLUSTER);
	}

	if(z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key", err.in_doubt);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	if(zval_to_as_policy_operate(z_operate_policy, &operate_policy,
			&operate_policy_p, &as_client->config.policies.operate) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid operate policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	} else {
		operate_policy_p = &operate_policy;
	}

	operations_size = zend_hash_num_elements(z_ops);
	if (!operations_size) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Empty operations array");
		goto CLEANUP;
	}
	set_serializer_from_policy_hash(&serializer_type, z_operate_policy);

	as_operations_inita(&ops, operations_size);
	operations_initialized = true;

	if (set_operations_generation_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		goto CLEANUP;
	}

	if (set_operations_ttl_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		goto CLEANUP;
	}

	if (!hashtable_is_list(z_ops)) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Operations array must be a list");
		goto CLEANUP;
	}

	zval* current_op = NULL;

	ZEND_HASH_FOREACH_VAL(z_ops, current_op)
	{

		if (Z_TYPE_P(current_op) != IS_ARRAY) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Operation must be an array");
			goto CLEANUP;
		}

		if (add_op_to_operations(Z_ARRVAL_P(current_op), &ops, &err, serializer_type) != AEROSPIKE_OK) {
			as_error_update(&err, err.code, NULL);
			goto CLEANUP;
		}

	}ZEND_HASH_FOREACH_END();

	if (aerospike_key_operate(as_client, &err, operate_policy_p, &key, &ops, &rec) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
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
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
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
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for operateOrdered", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	if (retval) {
		zval_dtor(retval);
		ZVAL_NULL(retval);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (!php_client || !php_client->as_client || !php_client->is_valid) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid aerospike object", false);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	as_client = php_client->as_client;

	if (!php_client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLUSTER, "No connection to Aerospike server", false);
		RETURN_LONG(AEROSPIKE_ERR_CLUSTER);
	}

	if(z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	if (z_operate_policy){
		if(zval_to_as_policy_operate(z_operate_policy, &operate_policy,
				&operate_policy_p, &as_client->config.policies.operate) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid operate policy");
			goto CLEANUP;
		}
	}

	operations_size = zend_hash_num_elements(z_ops);
	if (!operations_size) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Empty operations array");
		goto CLEANUP;
	}
	set_serializer_from_policy_hash(&serializer_type, z_operate_policy);

	as_operations_inita(&ops, operations_size);
	operations_initialized = true;

	if (set_operations_generation_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}


	if (set_operations_ttl_from_operate_policy(&ops, z_operate_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid TTL");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	zval* current_op = NULL;
	zend_string* string_key = NULL;

	ZEND_HASH_FOREACH_STR_KEY_VAL(z_ops, string_key, current_op)
	{
		if (string_key) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Operations array must not have string keys");
			err.code = AEROSPIKE_ERR_PARAM;
			goto CLEANUP;
		}

		if (Z_TYPE_P(current_op) != IS_ARRAY) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Operation must be an array");
			err.code = AEROSPIKE_ERR_PARAM;
			goto CLEANUP;
		}

		if (add_op_to_operations(Z_ARRVAL_P(current_op), &ops, &err, serializer_type) != AEROSPIKE_OK) {
			update_client_error(getThis(), err.code, err.message, err.in_doubt);
			goto CLEANUP;
		}

	}ZEND_HASH_FOREACH_END();

	if (aerospike_key_operate(as_client, &err, operate_policy_p, &key, &ops, &rec) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
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
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
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
	if (op_requires_bin(op_type)) {
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

	/* If it's a map operation, use the map op helper function */
	if (op_is_map_op(op_type)) {
		return add_map_op_to_operations(op_array, op_type, bin_name, ops, err, serializer_type);
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
		case AS_OPERATOR_DELETE: {
            if (!as_operations_add_delete(ops)) {
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

static as_status
add_map_op_to_operations(HashTable* op_array, int op_type, const char* bin_name,
		as_operations* ops, as_error* err, int serializer_type) {
	uint64_t count;
	int64_t index;
	int64_t rank;
	as_val* range_end = NULL;
	as_val* val = NULL;
	as_val* key = NULL;
	as_map_policy map_policy;
	as_map_return_type return_type;

	if (map_op_requires_count(op_type)) {
		if (get_count_from_op_hash(err, op_array, &count) != AEROSPIKE_OK) {
			return err->code;
		}
	}

	if (map_op_requires_index(op_type)) {
		if (get_index_from_op_hash(err, op_array, &index) != AEROSPIKE_OK) {
			return err->code;
		}
	}

	if (map_op_requires_rank(op_type)) {
		if (get_rank_from_op_hash(err, op_array, &rank) != AEROSPIKE_OK) {
			return err->code;
		}
	}

	if (map_op_requires_return_type(op_type)) {
		if (get_return_type_from_op_hash(err, op_array, &return_type) != AEROSPIKE_OK) {
			return err->code;
		}
	}

	if (map_op_requires_map_policy(op_type)) {
		if (get_map_policy_from_op_hash(err, op_array, &map_policy) != AEROSPIKE_OK) {
			return err->code;
		}
	}

	if (map_op_requires_key(op_type)) {
		if (get_key_from_op_hash(err, op_array, &key, serializer_type) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	if (map_op_requires_range_end(op_type)) {
		if (get_range_end_from_op_hash(err, op_array, &range_end, serializer_type) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	if (map_op_requires_val(op_type)) {
		if (get_value_from_op_hash(err, op_array, &val, serializer_type) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	switch(op_type) {

		/* 1 */
		case OP_MAP_SET_POLICY:
			if (!as_operations_add_map_set_policy(ops, bin_name, &map_policy)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_SET_POLICY operation");
			}
			break;
		/* 2 */
		case OP_MAP_PUT:
			if (!as_operations_add_map_put(ops, bin_name, &map_policy, key, val)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_PUT operation");
			}
			break;

		/* 3 */
		case OP_MAP_PUT_ITEMS: {
			as_map* map = NULL;
			if (as_val_type(val) != AS_MAP) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Value must be a hashtable for OP_MAP_PUT_ITEMS");
				goto CLEANUP;
			}
			map = as_map_fromval(val);
			if (!map) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to store map put items");
				goto CLEANUP;
			}
			if (!as_operations_add_map_put_items(ops, bin_name, &map_policy, map)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_PUT_ITEMS operation");
			}
			break;
		}

		/* 4 */
		case OP_MAP_INCREMENT:
			if (!as_operations_add_map_increment(ops, bin_name, &map_policy, key, val)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_INCREMENT operation");
			}
			break;

		/* 5 */
		case OP_MAP_DECREMENT:
			if (!as_operations_add_map_decrement(ops, bin_name, &map_policy, key, val)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_INCREMENT operation");
			}
			break;

		/* 6 */
		case OP_MAP_SIZE:
			if (!as_operations_add_map_size(ops, bin_name)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_SIZE operation");
			}
			break;

		/* 7 */
		case OP_MAP_CLEAR:
			if (!as_operations_add_map_clear(ops, bin_name)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_CLEAR operation");
			}
			break;

		/* 8 */
		case OP_MAP_REMOVE_BY_KEY:
			if (!as_operations_add_map_remove_by_key(ops, bin_name, key, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_KEY operation");
			}
			break;

		/* 9 */
		case OP_MAP_REMOVE_BY_KEY_LIST: {
			as_list* key_list = NULL;
			if (as_val_type(key) != AS_LIST) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "list of keys must be an array");
				goto CLEANUP;
			}
			key_list = as_list_fromval(key);

			if (!key_list) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to store key list for remove by key list");
				goto CLEANUP;
			}
			if (!as_operations_add_map_remove_by_key_list(ops, bin_name, key_list, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_KEY_LIST operation");
			}

			break;
		}

		/* 10 */
		case OP_MAP_REMOVE_BY_KEY_RANGE:
			if (!as_operations_add_map_remove_by_key_range(ops, bin_name, key, range_end, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_KEY_RANGE operation");
			}
			break;

		/* 11 */
		case OP_MAP_REMOVE_BY_VALUE:
			if (!as_operations_add_map_remove_by_value(ops, bin_name, val, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_VALUE operation");
			}
			break;

		/* 12 */
		case OP_MAP_REMOVE_BY_VALUE_LIST: {
			as_list* val_list = NULL;
			if (as_val_type(val) != AS_LIST) {
				as_error_update(err, AEROSPIKE_ERR_PARAM, "list of values must be an array");
				goto CLEANUP;
			}
			val_list = as_list_fromval(val);

			if (!val_list) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to store key list for remove by value list");
				goto CLEANUP;
			}
			if (!as_operations_add_map_remove_by_value_list(ops, bin_name, val_list, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_VALUE_LIST operation");
			}
			break;
		}

		/* 13 */
		case OP_MAP_REMOVE_BY_VALUE_RANGE:
			if (!as_operations_add_map_remove_by_value_range(ops, bin_name, val, range_end, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_VALUE_RANGE operation");
			}
			break;

		/* 14 */
		case OP_MAP_REMOVE_BY_INDEX:
			if (!as_operations_add_map_remove_by_index(ops, bin_name, index, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_INDEX operation");
			}
			break;

		/* 15 */
		case OP_MAP_REMOVE_BY_INDEX_RANGE:
			if (!as_operations_add_map_remove_by_index_range(ops, bin_name, index, count, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_INDEX_RANGE operation");
			}
			break;

		/* 16 */
		case OP_MAP_REMOVE_BY_RANK:
			if (!as_operations_add_map_remove_by_rank(ops, bin_name, rank, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_RANK operation");
			}
			break;

		/* 17 */
		case OP_MAP_REMOVE_BY_RANK_RANGE:
			if (!as_operations_add_map_remove_by_rank_range(ops, bin_name, rank, count, return_type)) {
				return as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_REMOVE_BY_RANK_RANGE operation");
			}
			break;

		/* 18 */
		case OP_MAP_GET_BY_KEY:
			if (!as_operations_add_map_get_by_key(ops, bin_name, key, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_KEY operation");
			}
			break;

		/* 19 */
		case OP_MAP_GET_BY_KEY_RANGE:
			if (!as_operations_add_map_get_by_key_range(ops, bin_name, key, range_end, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_KEY_RANGE operation");
			}
			break;

		/* 20 */
		case OP_MAP_GET_BY_VALUE:
			if (!as_operations_add_map_get_by_value(ops, bin_name, val, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_VALUE operation");
			}
			break;

		/* 21 */
		case OP_MAP_GET_BY_VALUE_RANGE:
			if (!as_operations_add_map_get_by_value_range(ops, bin_name, val, range_end, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_VALUE_RANGE operation");
			}
			break;

		/* 22 */
		case OP_MAP_GET_BY_INDEX:
			if (!as_operations_add_map_get_by_index(ops, bin_name, index, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_INDEX operation");
			}
			break;

		/* 23 */
		case OP_MAP_GET_BY_INDEX_RANGE:
			if (!as_operations_add_map_get_by_index_range(ops, bin_name, index, count, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_INDEX_RANGE operation");
			}
			break;

		/* 24 */
		case OP_MAP_GET_BY_RANK:
			if (!as_operations_add_map_get_by_rank(ops, bin_name, rank, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_RANK operation");
			}
			break;

		/* 25 */
		case OP_MAP_GET_BY_RANK_RANGE:
			if (!as_operations_add_map_get_by_rank_range(ops, bin_name, rank, count, return_type)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Failed to add OP_MAP_GET_BY_RANK_RANGE operation");
			}
			break;
		default:
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Unknown map operation");
			break;
	}

CLEANUP:

/*
 * 	as_val* range_end = NULL;
	as_val* val = NULL;
	as_val* key = NULL;
 */
	if (err->code != AEROSPIKE_OK) {
		if (range_end) {
			as_val_destroy(range_end);
		}
		if (val) {
			as_val_destroy(val);
		}
		if (key) {
			as_val_destroy(key);
		}
	}
	return err->code;

}

static inline bool op_requires_bin(int op_type) {
	return (op_type != AS_OPERATOR_DELETE);
}


static inline bool op_requires_val(int op_type) {
	return (op_type != AS_OPERATOR_TOUCH && op_type != OP_LIST_CLEAR  &&
			op_type != AS_OPERATOR_READ  && op_type != OP_LIST_SIZE   &&
			op_type != OP_LIST_POP       && op_type != OP_LIST_REMOVE &&
			op_type != OP_LIST_GET       && op_type != OP_MAP_SIZE    &&
			op_type != OP_MAP_CLEAR      && op_type != AS_OPERATOR_DELETE);
}

static inline bool
op_is_map_op(int op_type) {
	return (
			 op_type == OP_MAP_SET_POLICY ||
			 op_type == OP_MAP_PUT ||
			 op_type == OP_MAP_PUT_ITEMS ||
			 op_type == OP_MAP_INCREMENT ||
			 op_type == OP_MAP_DECREMENT ||
			 op_type == OP_MAP_SIZE ||
			 op_type == OP_MAP_CLEAR ||
			 op_type == OP_MAP_REMOVE_BY_KEY ||
			 op_type == OP_MAP_REMOVE_BY_KEY_LIST ||
			 op_type == OP_MAP_REMOVE_BY_KEY_RANGE ||
			 op_type == OP_MAP_REMOVE_BY_VALUE ||
			 op_type == OP_MAP_REMOVE_BY_VALUE_LIST ||
			 op_type == OP_MAP_REMOVE_BY_VALUE_RANGE ||
			 op_type == OP_MAP_REMOVE_BY_INDEX ||
			 op_type == OP_MAP_REMOVE_BY_INDEX_RANGE ||
			 op_type == OP_MAP_REMOVE_BY_RANK ||
			 op_type == OP_MAP_REMOVE_BY_RANK_RANGE ||
			 op_type == OP_MAP_GET_BY_KEY ||
			 op_type == OP_MAP_GET_BY_KEY_RANGE ||
			 op_type == OP_MAP_GET_BY_VALUE ||
			 op_type == OP_MAP_GET_BY_VALUE_RANGE ||
			 op_type == OP_MAP_GET_BY_INDEX ||
			 op_type == OP_MAP_GET_BY_INDEX_RANGE ||
			 op_type == OP_MAP_GET_BY_RANK ||
			 op_type == OP_MAP_GET_BY_RANK_RANGE);
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


/* Map op argument classifiers */
static inline bool
map_op_requires_map_return_type(int op_type) {
	return (
			op_type == OP_MAP_REMOVE_BY_KEY ||
			op_type == OP_MAP_REMOVE_BY_KEY_LIST ||
			op_type == OP_MAP_REMOVE_BY_KEY_RANGE ||
			op_type == OP_MAP_REMOVE_BY_VALUE ||
			op_type == OP_MAP_REMOVE_BY_VALUE_LIST ||
			op_type == OP_MAP_REMOVE_BY_VALUE_RANGE ||
			op_type == OP_MAP_REMOVE_BY_INDEX ||
			op_type == OP_MAP_REMOVE_BY_INDEX_RANGE ||
			op_type == OP_MAP_REMOVE_BY_RANK ||
			op_type == OP_MAP_REMOVE_BY_RANK_RANGE ||
			op_type == OP_MAP_GET_BY_KEY ||
			op_type == OP_MAP_GET_BY_KEY_RANGE ||
			op_type == OP_MAP_GET_BY_VALUE ||
			op_type == OP_MAP_GET_BY_VALUE_RANGE ||
			op_type == OP_MAP_GET_BY_INDEX ||
			op_type == OP_MAP_GET_BY_INDEX_RANGE ||
			op_type == OP_MAP_GET_BY_RANK ||
			op_type == OP_MAP_GET_BY_RANK_RANGE);

}

static inline bool map_op_requires_key(int op_type) {
	return (
			op_type == OP_MAP_PUT ||
			op_type == OP_MAP_INCREMENT ||
			op_type == OP_MAP_DECREMENT ||
			op_type == OP_MAP_REMOVE_BY_KEY ||
			op_type == OP_MAP_REMOVE_BY_KEY_LIST ||
			op_type == OP_MAP_REMOVE_BY_KEY_RANGE ||
			op_type == OP_MAP_GET_BY_KEY ||
			op_type == OP_MAP_GET_BY_KEY_RANGE
			);

}

static inline bool map_op_requires_val(int op_type) {
	return (
			op_type == OP_MAP_PUT ||
			op_type == OP_MAP_INCREMENT ||
			op_type == OP_MAP_DECREMENT ||
			op_type == OP_MAP_REMOVE_BY_VALUE ||
			op_type == OP_MAP_REMOVE_BY_VALUE_LIST ||
			op_type == OP_MAP_REMOVE_BY_VALUE_RANGE ||
			op_type == OP_MAP_GET_BY_VALUE ||
			op_type == OP_MAP_GET_BY_VALUE_RANGE ||
			op_type == OP_MAP_PUT_ITEMS);
}

static inline bool map_op_requires_rank(int op_type) {
	return (
			op_type == OP_MAP_REMOVE_BY_RANK ||
			op_type == OP_MAP_REMOVE_BY_RANK_RANGE ||
			op_type == OP_MAP_GET_BY_RANK ||
			op_type == OP_MAP_GET_BY_RANK_RANGE);
}

static inline bool map_op_requires_return_type(int op_type) {
	return (
			op_type == OP_MAP_REMOVE_BY_KEY ||
			op_type == OP_MAP_REMOVE_BY_KEY_LIST ||
			op_type == OP_MAP_REMOVE_BY_KEY_RANGE ||
			op_type == OP_MAP_REMOVE_BY_VALUE ||
			op_type == OP_MAP_REMOVE_BY_VALUE_LIST ||
			op_type == OP_MAP_REMOVE_BY_VALUE_RANGE ||
			op_type == OP_MAP_REMOVE_BY_INDEX ||
			op_type == OP_MAP_REMOVE_BY_INDEX_RANGE ||
			op_type == OP_MAP_REMOVE_BY_RANK ||
			op_type == OP_MAP_REMOVE_BY_RANK_RANGE ||
			op_type == OP_MAP_GET_BY_KEY ||
			op_type == OP_MAP_GET_BY_KEY_RANGE ||
			op_type == OP_MAP_GET_BY_VALUE ||
			op_type == OP_MAP_GET_BY_VALUE_RANGE ||
			op_type == OP_MAP_GET_BY_INDEX ||
			op_type == OP_MAP_GET_BY_INDEX_RANGE ||
			op_type == OP_MAP_GET_BY_RANK ||
			op_type == OP_MAP_GET_BY_RANK_RANGE);
}

static inline bool map_op_requires_map_policy(int op_type) {
	return (
			op_type == OP_MAP_SET_POLICY ||
			op_type == OP_MAP_PUT ||
			op_type == OP_MAP_PUT_ITEMS ||
			op_type == OP_MAP_INCREMENT ||
			op_type == OP_MAP_DECREMENT);
}

static inline bool map_op_requires_index(int op_type) {
	return (
			op_type == OP_MAP_REMOVE_BY_INDEX ||
			op_type == OP_MAP_REMOVE_BY_INDEX_RANGE ||
			op_type == OP_MAP_GET_BY_INDEX ||
			op_type == OP_MAP_GET_BY_INDEX_RANGE);
}

static inline bool map_op_requires_count(int op_type) {
	return (
			op_type == OP_MAP_REMOVE_BY_INDEX_RANGE ||
			op_type == OP_MAP_REMOVE_BY_RANK_RANGE ||
			op_type == OP_MAP_GET_BY_INDEX_RANGE ||
			op_type == OP_MAP_GET_BY_RANK_RANGE);
}

static inline bool map_op_requires_range_end(int op_type) {
	return (
			op_type == OP_MAP_REMOVE_BY_KEY_RANGE ||
			op_type == OP_MAP_REMOVE_BY_VALUE_RANGE ||
			op_type == OP_MAP_GET_BY_KEY_RANGE ||
			op_type == OP_MAP_GET_BY_VALUE_RANGE);
}

static as_status get_count_from_op_hash(as_error* err, HashTable* op_hash, uint64_t* count) {
	zval* z_count = NULL;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_count = zend_hash_str_find(op_hash, AS_MAP_COUNT_KEY, strlen(AS_MAP_COUNT_KEY));
	if (!z_count) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required count entry");
	}

	if (Z_TYPE_P(z_count) != IS_LONG) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Count entry must be a long type");
	}

	if (Z_LVAL_P(z_count) < 0) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Count must not be negative");
	}

	*count = Z_LVAL_P(z_count);
	return AEROSPIKE_OK;
}

static as_status get_index_from_op_hash(as_error* err, HashTable* op_hash, int64_t* index) {
	zval* z_index = NULL;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_index = zend_hash_str_find(op_hash, AS_MAP_INDEX_KEY, strlen(AS_MAP_INDEX_KEY));
	if (!z_index) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required index entry");
	}

	if (Z_TYPE_P(z_index) != IS_LONG) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Index entry must be a long type");
	}

	*index = Z_LVAL_P(z_index);
	return AEROSPIKE_OK;
}

static as_status get_rank_from_op_hash(as_error* err, HashTable* op_hash, int64_t* rank) {
	zval* z_rank = NULL;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_rank = zend_hash_str_find(op_hash, AS_MAP_RANK_KEY, strlen(AS_MAP_RANK_KEY));
	if (!z_rank) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required rank entry");
	}

	if (Z_TYPE_P(z_rank) != IS_LONG) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "rank entry must be a long type");
	}

	*rank = Z_LVAL_P(z_rank);
	return AEROSPIKE_OK;
}

/* This is limited for now, This really should allow any of the primitive as_vals, but that causes
 * some weird issues with PHP converting them back to arrays :(, At the very least, this should take
 * an int*/
static as_status get_key_from_op_hash(as_error* err, HashTable* op_hash, as_val** key, int serializer_type) {
	zval* z_key = NULL;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_key = zend_hash_str_find(op_hash, AS_MAP_KEY_KEY, strlen(AS_MAP_KEY_KEY));
	if (!z_key) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required key entry");
	}

	if (zval_to_as_val(z_key, key, err, serializer_type) != AEROSPIKE_OK) {
		return err->code;
	}
	return AEROSPIKE_OK;
}

static as_status get_value_from_op_hash(as_error* err, HashTable* op_hash, as_val** val, int serializer_type) {
	zval* z_value = NULL;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_value = zend_hash_str_find(op_hash, AS_MAP_VALUE_KEY, strlen(AS_MAP_VALUE_KEY));
	if (!z_value) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required val entry");
	}

	if (zval_to_as_val(z_value, val, err, serializer_type) != AEROSPIKE_OK) {
		return err->code;
	}

	return AEROSPIKE_OK;
}

static as_status get_range_end_from_op_hash(as_error* err, HashTable* op_hash, as_val** range_end, int serializer_type) {
	zval* z_range_end = NULL;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_range_end = zend_hash_str_find(op_hash, AS_MAP_RANGE_END, strlen(AS_MAP_RANGE_END));
	if (!z_range_end) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required range_end entry");
	}

	if (zval_to_as_val(z_range_end, range_end, err, serializer_type) != AEROSPIKE_OK) {
		return err->code;
	}

	return AEROSPIKE_OK;
}

static as_status get_map_policy_from_op_hash(as_error* err, HashTable* op_hash, as_map_policy* map_policy_p) {
	zval* z_map_policy = NULL;
	as_status status = AEROSPIKE_OK;
	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_map_policy = zend_hash_str_find(op_hash, AS_MAP_POLICY_KEY, strlen(AS_MAP_POLICY_KEY));
	if (!z_map_policy) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required map_policy entry");
	}

	status = zval_to_as_policy_map(z_map_policy, map_policy_p);
	if (status != AEROSPIKE_OK) {
		return as_error_update(err, status, "Invalid map_policy");
	}
	return AEROSPIKE_OK;
}

static as_status get_return_type_from_op_hash(as_error* err, HashTable* op_hash, as_map_return_type* return_type) {
	zval* z_return_type = NULL;

	if (!op_hash) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation must not be empty");
	}

	z_return_type = zend_hash_str_find(op_hash, AS_MAP_RETURN_TYPE_KEY, strlen(AS_MAP_RETURN_TYPE_KEY));
	if (!z_return_type) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "Operation missing a required return_type entry");
	}

	if (Z_TYPE_P(z_return_type) != IS_LONG) {
		return as_error_update(err, AEROSPIKE_ERR_PARAM, "return_type must be a long");
	}
	*return_type = (as_map_return_type)Z_LVAL_P(z_return_type);

	return AEROSPIKE_OK;
}
