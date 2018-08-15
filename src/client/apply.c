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
#include "policy_conversions.h"
#include "php_aerospike_types.h"
#include "conversions.h"


/* {{{ proto int Aerospike::apply( array key, string module, string function[, array args [, mixed &returned [, array options ]]] )
    Applies a UDF to a record  */
PHP_METHOD(Aerospike, apply) {
	as_error err;
	aerospike* as_client = NULL;
	AerospikeClient* php_client = NULL;
	as_policy_apply apply_policy;
	as_policy_apply* apply_policy_p = NULL;
	as_key key;
	char* function = NULL;
	char* module = NULL;
	size_t function_len;
	size_t module_len;
	as_list* arg_list = NULL;
	as_val* result = NULL;
	zval* z_returned = NULL;
	zval* z_policy_apply = NULL;
	HashTable* z_key = NULL;
	HashTable* z_args = NULL;
	bool key_initialized = false;
	bool args_initialized = false;
	bool has_return = false;
	int serializer_type = INI_INT("aerospike.serializer");
	as_error_init(&err);

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	reset_client_error(getThis());

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hss|h!z/z",
			&z_key, &module, &module_len, &function, &function_len,
			&z_args, &z_returned, &z_policy_apply) != SUCCESS) {
				update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters for apply", false);
				RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	// Clear return reference if provided
	if (z_returned) {
		zval_dtor(z_returned);
		ZVAL_NULL(z_returned);
		has_return = true;
	}

	//Convert key
	if (z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid key.");
		goto CLEANUP;
	}
	key_initialized = true;

	if (zval_to_as_policy_apply(z_policy_apply, &apply_policy,
			&apply_policy_p, &as_client->config.policies.apply) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid Policy.");
		goto CLEANUP;
	}

	if (set_serializer_from_policy_hash(&serializer_type, z_policy_apply) != AEROSPIKE_OK ) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid serializer type");
		goto CLEANUP;
	}

	if (z_args) {
		if (z_hashtable_to_as_list(z_args, &arg_list, &err, serializer_type) != AEROSPIKE_OK) {
			as_error_update(&err, err.code, err.message);
			goto CLEANUP;
		}
	} else {
		arg_list = (as_list*)as_arraylist_new(0, 0);
	}
	args_initialized = true;

	if (aerospike_key_apply(as_client, &err, apply_policy_p, &key, module, function, arg_list, &result) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (has_return) {
		if (as_val_to_zval(result, z_returned, &err) != AEROSPIKE_OK) {
			if (z_returned) {
				zval_dtor(z_returned);
				ZVAL_NULL(z_returned);
			}
		}
	}

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (key_initialized) {
		as_key_destroy(&key);
	}
	if (args_initialized) {
		as_list_destroy(arg_list);
	}
	if (result) {
		as_val_destroy(result);
	}
	RETURN_LONG(err.code);
}
/* }}} */
