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

/* {{{ proto int Aerospike::removeBin( array key, array bins [, array options ])
    Removes a bin from a record */
PHP_METHOD(Aerospike, removeBin)
{
	as_key key;
	as_error err;
	as_error_init(&err);
	as_record rec;

	bool record_initialized = false;
	bool key_initialized = false;

	as_policy_write write_policy;
	as_policy_write* write_policy_p = NULL;

	HashTable* z_key = NULL;
	HashTable* bin_array = NULL;
	zval* z_write_policy = NULL;

	AerospikeClient* client = NULL;
	aerospike* as_ptr = NULL;

	char* bin_name = NULL;
	size_t num_bins;
	zval* php_bin_name = NULL;

	reset_client_error(getThis());
	client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}
	as_ptr = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hh|z", &z_key, &bin_array, &z_write_policy)
		!= SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to removeBin", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_write(z_write_policy, &write_policy,
			&write_policy_p, &as_ptr->config.policies.write) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy to removeBin", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (z_hashtable_to_as_key(z_key, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	num_bins = zend_hash_num_elements(bin_array);
	as_record_inita(&rec, num_bins);
	record_initialized = true;

	as_status gen_status = set_record_generation_from_write_policy(&rec, z_write_policy);
	if (gen_status != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "generation policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (!hashtable_is_list(bin_array)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "list of bins must be an array", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	/* Bins are set to nil by storing a nil in them */
	ZEND_HASH_FOREACH_VAL(bin_array, php_bin_name)
	{
		bin_name = Z_STRVAL_P(php_bin_name);
		as_record_set_nil(&rec, bin_name);
	}ZEND_HASH_FOREACH_END();

	as_error_init(&err);

	as_status status = aerospike_key_put(as_ptr, &err, write_policy_p, &key, &rec);

	if (record_initialized) {
		as_record_destroy(&rec);
	}
	if (key_initialized) {
		as_key_destroy(&key);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(status);

}
/* }}} */
