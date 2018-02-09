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

/* {{{ proto int Aerospike::remove( array key [, array options ] )
    Removes a record from the cluster */
PHP_METHOD(Aerospike, remove)
{
	as_error err;
	as_error_init(&err);
	as_key key;
	bool key_initialized = false;

	as_policy_remove remove_policy;
	as_policy_remove* remove_policy_p;

	HashTable* z_key = NULL;
	zval* z_remove_options = NULL;

	AerospikeClient* client = NULL;
	aerospike* as_ptr = NULL;

	reset_client_error(getThis());
	client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}
	as_ptr = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "h|z", &z_key, &z_remove_options) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for remove", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_remove(z_remove_options, &remove_policy,
			&remove_policy_p, &as_ptr->config.policies.remove) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid remove policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	as_error_init(&err);

	if (z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, "Failed to create key", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	as_status status = aerospike_key_remove(as_ptr, &err, remove_policy_p, &key);

	if (key_initialized) {
		as_key_destroy(&key);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(status);

}
/* }}} */
