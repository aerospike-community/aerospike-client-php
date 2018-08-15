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


/* {{{ proto int Aerospike::touch( array key, int ttl=0 [, array options ] )
    Touch a record, incrementing its generation and setting its time-to-live */
PHP_METHOD(Aerospike, touch)
{
	as_key key;
	as_error err;
	bool key_initialized = false;
	bool operations_initialized = false;
	as_record* rec = NULL; // This should be untouched by the operation call
	as_operations operations;
	as_policy_operate operate_policy;
	as_policy_operate* operate_policy_p = NULL;
	HashTable* z_key = NULL;
	zval* z_operations_policy = NULL;
	zend_long ttl_value = 0;
	as_error_init(&err);

	reset_client_error(getThis());
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	if (!client || !client->is_valid || !client->as_client) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Invalid Aerospike object", false);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);

	}

	if (!client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "No connection to Server", false);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	aerospike* as_ptr = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "h|lz", &z_key, &ttl_value, &z_operations_policy)
		!= SUCCESS) {
		return;
	}

	if (z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Unable to convert key");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}
	key_initialized = true;

	if (zval_to_as_policy_operate(z_operations_policy, &operate_policy,
			&operate_policy_p, &as_ptr->config.policies.operate) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid operate policy");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}
	operate_policy_p = &operate_policy;

	as_operations_inita(&operations, 1);
	operations_initialized = true;

	as_operations_add_touch(&operations);
	operations.ttl = ttl_value;

	if (set_operations_generation_from_operate_policy(&operations, z_operations_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid generation policy");
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
