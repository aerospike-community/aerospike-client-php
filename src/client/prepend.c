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
#include "conversions.h"
#include "php_aerospike_types.h"
#include "policy_conversions.h"


/* {{{ proto int Aerospike::prepend( array key, string bin, string value [, array options ] )
    Prepends a string to an existing bin's string value */
PHP_METHOD(Aerospike, prepend)
{
	as_key key;
	as_error err;
	as_error_init(&err);
	as_operations operations;
	as_policy_operate operate_policy;
	as_policy_operate* operate_policy_p = NULL;
	as_record* rec = NULL; // This should be untouched by the operation call

	bool key_initialized = false;
	bool operations_initialized = false;

	HashTable* z_key = NULL;
	zval* z_op_policy = NULL;
	size_t bin_len, prepend_str_len;
	char* bin_str = NULL;
	char* prepend_str = NULL;

	reset_client_error(getThis());

	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (!client || !client->is_valid || !client->as_client) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid Aerospike object", false);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}

	if (!client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Not connected to Aerospike server", false);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	aerospike* as_ptr = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hss|z", &z_key, &bin_str,
							  &bin_len, &prepend_str, &prepend_str_len,
							  &z_op_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to prepend", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Bin name is too long", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);	
	}

	if (z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	if (zval_to_as_policy_operate(z_op_policy, &operate_policy,
			&operate_policy_p, &as_ptr->config.policies.operate) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid operate policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	operate_policy_p = &operate_policy;

	as_operations_inita(&operations, 1);
	operations_initialized = true;
	as_operations_add_prepend_str(&operations, bin_str, prepend_str);

	if (set_operations_generation_from_operate_policy(&operations, z_op_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid generation policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	if (set_operations_ttl_from_operate_policy(&operations, z_op_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid TTL");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	as_status status = aerospike_key_operate(as_ptr, &err, operate_policy_p, &key, &operations, &rec);
	if (status != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		goto CLEANUP;
	}

CLEANUP:

	if (operations_initialized) {
		as_operations_destroy(&operations);
	}

	if (key_initialized) {
		as_key_destroy(&key);
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
