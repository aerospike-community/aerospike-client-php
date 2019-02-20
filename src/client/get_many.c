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

#include "aerospike/as_error.h"
#include "aerospike_class.h"
#include "policy_conversions.h"
#include "php_aerospike_types.h"
#include "aerospike/aerospike_batch.h"
#include "conversions.h"
#include "aerospike/as_bin.h"


as_status get_many_with_batch_read(aerospike* as, as_error* err, const as_policy_batch* policy,
		char** bins, uint32_t bin_count, HashTable* z_keys, zval* z_records);

/*
 * These function support the getMany calls, based on whether batch direct is being used,
 * two separate helper functions are called, one utilizes a callback passed to aerospike_batch_get
 * the other handles the stored records from aerospike_batch_read
 */


/* {{{ proto int Aerospike::getMany ( array $keys, array &$records [, array $filter [, array $options]] ) */
PHP_METHOD(Aerospike, getMany) {


	/* parameter variables */
	HashTable* z_keys = NULL;
	zval* z_records = NULL;
	HashTable* z_filter = NULL;
	zval* z_policy = NULL;

	char** bins = NULL;
	as_policy_batch batch_policy;
	as_policy_batch* batch_policy_p = NULL;

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	uint32_t bin_count = 0;
	as_error err;
	as_error_init(&err);
	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hz/|h!z",
			&z_keys, &z_records, &z_filter, &z_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to getMany", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zval_dtor(z_records);
	ZVAL_NULL(z_records);

	if (z_policy) {
		if (zval_to_as_policy_batch(z_policy, &batch_policy,
				&batch_policy_p, &as_client->config.policies.batch) != AEROSPIKE_OK) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid batch policy", false);
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
		}
		batch_policy_p = &batch_policy;
	}

	/* Transform the php filter bins into char** */
	if (z_filter && zend_hash_num_elements(z_filter)) {
		int num_elements = zend_hash_num_elements(z_filter);
		bin_count = num_elements;
		bins = (char**)alloca(num_elements * sizeof(char*));

		char* bin_name = NULL;
		zval* z_bin_name = NULL;
		int i = 0;
		ZEND_HASH_FOREACH_VAL(z_filter, z_bin_name) {

			if (!z_bin_name || (Z_TYPE_P(z_bin_name) != IS_STRING) ) {
				as_error_update(&err, AEROSPIKE_ERR_PARAM, "Bin names must be strings");
				goto CLEANUP;
			}

			if (Z_STRLEN_P(z_bin_name) > AS_BIN_NAME_MAX_LEN) {
				as_error_update(&err, AEROSPIKE_ERR_PARAM, "Bin name too long");
				goto CLEANUP;
			}

			bin_name = (char*)alloca(Z_STRLEN_P(z_bin_name) + 1);
			strcpy(bin_name, Z_STRVAL_P(z_bin_name));
			bins[i] = bin_name;
			i++;
		}ZEND_HASH_FOREACH_END();
	}


	get_many_with_batch_read(as_client, &err, batch_policy_p, bins, bin_count, z_keys, z_records);


CLEANUP:

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */

as_status get_many_with_batch_read(aerospike* as, as_error* err, const as_policy_batch* policy,
		char** bins, uint32_t bin_count, HashTable* z_keys, zval* z_records) {

	int num_records;
	zval z_get_entry;
	zval z_key_entry;
	zval z_record_entry;
	bool records_initialized = false;

	as_batch_read_records records;
	as_batch_read_record* record;

	num_records = zend_hash_num_elements(z_keys);
	array_init(z_records);

	as_batch_read_init(&records, num_records);
	records_initialized = true;

	zval* z_key  = NULL;
	ZEND_HASH_FOREACH_VAL(z_keys, z_key) {
		if (!z_key || (Z_TYPE_P(z_key) != IS_ARRAY)) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Keys must be arrays");
			goto CLEANUP;
		}
		record = as_batch_read_reserve(&records);
		if (bins) {
			record->bin_names = bins;
			record->n_bin_names = bin_count;
		} else {
			record->read_all_bins = true;
		}

		if (z_hashtable_to_as_key(Z_ARRVAL_P(z_key), &record->key, err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}ZEND_HASH_FOREACH_END();

	if (aerospike_batch_read(as, err, policy, &records) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	for (int i = 0; i < num_records; i++) {
		record = (as_batch_read_record*)as_vector_get(&records.list, i);

		if (record->result == AEROSPIKE_ERR_RECORD_NOT_FOUND) {

			if (as_key_to_zval(&record->key, &z_key_entry, true, err) != AEROSPIKE_OK) {
				goto CLEANUP;
			}

			array_init(&z_get_entry);
			add_assoc_zval(&z_get_entry, "key", &z_key_entry);
			add_assoc_null(&z_get_entry, "metadata");
			add_assoc_null(&z_get_entry, "bins");

			add_next_index_zval(z_records, &z_get_entry);

		} else {
			if (as_record_to_zval(&record->record, &z_record_entry, &record->key, true, err) != AEROSPIKE_OK) {
				goto CLEANUP;
			}
			add_next_index_zval(z_records, &z_record_entry);
		}
	}

CLEANUP:

	if (records_initialized) {
		as_batch_read_destroy(&records);
	}
	if (err->code != AEROSPIKE_OK) {
		zval_dtor(z_records);
		ZVAL_NULL(z_records);
	}
	return err->code;
}
