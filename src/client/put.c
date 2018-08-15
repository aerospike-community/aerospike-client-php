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


/* {{{ proto int Aerospike::put( array key, array record [, int ttl=0 [, array options ]] )
    Writes a record to the cluster */
PHP_METHOD(Aerospike, put)
{

	as_key key;
	bool key_initialized = false;

	as_error err;
	as_error_init(&err);

	HashTable* z_key_hash = NULL;
	HashTable* z_record_hash;
	zval* zval_to_store = NULL;
	zval* z_write_policy = NULL;
	zval* z_ttl = NULL;

	as_policy_write write_policy;
	as_policy_write* write_policy_p = NULL;

	as_record* record = NULL;

	int serializer_type = INI_INT("aerospike.serializer");

	reset_client_error(getThis());
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = client->as_client;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hz|z!z",
			&z_key_hash, &zval_to_store, &z_ttl, &z_write_policy) != SUCCESS) {

		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to put", false);
	  	RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (z_ttl && !(Z_TYPE_P(z_ttl) == IS_NULL || Z_TYPE_P(z_ttl) == IS_LONG)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "ttl must be null or long", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (z_ttl && Z_TYPE_P(z_ttl) == IS_NULL) {
		z_ttl = NULL;
	}

	if (Z_TYPE_P(zval_to_store) != IS_ARRAY) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Record must be an array", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	z_record_hash = Z_ARRVAL_P(zval_to_store);

	if (!zend_hash_num_elements(z_record_hash)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Array of bins must not be empty", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (z_write_policy) {
		if (zval_to_as_policy_write(z_write_policy, &write_policy,
				&write_policy_p, &as_ptr->config.policies.write) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy");
			goto CLEANUP;
		}
	}

	if (z_hashtable_to_as_key(z_key_hash, &key, &err) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid Key");
		goto CLEANUP;
	}
	key_initialized = true;

	if (set_serializer_from_policy_hash(&serializer_type, z_write_policy) != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid serializer value");
		goto CLEANUP;
	}

	if (z_hashtable_to_as_record(z_record_hash, &record, &err, serializer_type) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (z_ttl) {
		record->ttl = (uint32_t)Z_LVAL_P(z_ttl);
	}

	as_status gen_status = set_record_generation_from_write_policy(record, z_write_policy);
	if (gen_status != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "generation policy");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	/* Arguments validated and ready, call the C client function */
    aerospike_key_put(as_ptr, &err, write_policy_p, &key, record);

CLEANUP:
    if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
    }
    if (key_initialized) {
	as_key_destroy(&key);
    }
    if (record) {
    	as_record_destroy(record);
    }
	RETURN_LONG(err.code);
}
/* }}} */
