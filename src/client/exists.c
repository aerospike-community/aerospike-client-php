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


/* {{{ proto int Aerospike::exists( array key, array &metadata [, array options] )
    Returns a record's metadata. Sets record_not_found if the key does not exist */
PHP_METHOD(Aerospike, exists)
{
	as_key key;
	as_error err;
	as_error_init(&err);
	zval* metadata;
	as_record* record = NULL;
	HashTable* z_key_hash = NULL;
	zval* z_read_policy = NULL;

	reset_client_error(getThis());
	as_policy_read read_policy;
	as_policy_read* read_policy_p = NULL;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}

	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hz/|z",
		&z_key_hash, &metadata, &z_read_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to exists", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	as_error_init(&err);

	if (z_hashtable_to_as_key(z_key_hash, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Failed to convert key", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	// Because this is a reference, we need to destroy the old value before reusing it.
	// null is also a reasonable return value in case of failure.
	zval_dtor(metadata);
	ZVAL_NULL(metadata);

	if (zval_to_as_policy_read(z_read_policy, &read_policy,
			&read_policy_p, &as_ptr->config.policies.read) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid read policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	read_policy_p = &read_policy;

    aerospike_key_exists(as_ptr, &err, read_policy_p, &key, &record);
	as_key_destroy(&key);
	
	if (record) {
		array_init(metadata);
		as_record_to_z_metadata(record, metadata, &err);
		as_record_destroy(record);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		zval_dtor(metadata);
		ZVAL_NULL(metadata);
	}

	RETURN_LONG(err.code);
}
/* }}} */
