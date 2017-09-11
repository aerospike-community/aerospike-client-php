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

//	aerospike_key_select(self->as, &err, read_policy_p, &key, (const char **) bins, &rec);

PHP_METHOD(Aerospike, select)
{
	as_key key;
	as_error err;
	zval* select_bins = NULL;
	zval* z_read_policy = NULL;
	zval* z_record = NULL;
	HashTable* z_key_hash;

	as_policy_read read_policy;
	as_policy_read* read_policy_p = NULL;
	int bin_count = 0;
	as_status status = AEROSPIKE_OK;
	as_record* record = NULL;
	char** filter_bins = NULL;
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = client->as_client;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hzz/|z",
		&z_key_hash, &select_bins, &z_record, &z_read_policy) != SUCCESS) {
	  RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	zval_dtor(z_record);
	ZVAL_NULL(z_record);

	if (z_hashtable_to_as_key(z_key_hash, &key, &err) != AEROSPIKE_OK) {
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_select_bins(select_bins, &filter_bins, &bin_count) != AEROSPIKE_OK) {
		 status = AEROSPIKE_ERR_PARAM;
		 goto CLEANUP;
	}


	if (zval_to_as_policy_read(z_read_policy, &read_policy, &read_policy_p, &as_ptr->config.policies.read) != AEROSPIKE_OK) {
		 status = AEROSPIKE_ERR_PARAM;
		 goto CLEANUP;
	}

	as_error_init(&err);
	status = aerospike_key_select(as_ptr, &err, read_policy_p,
								  &key, (const char **)filter_bins, &record);

	if (status != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (as_record_to_zval(record, z_record, &key, false, &err) != AEROSPIKE_OK) {
		status = err.code; // Really is something else
		goto CLEANUP;
	}


CLEANUP:
	if (record) {
		as_record_destroy(record);
	}

	as_key_destroy(&key);
	if (filter_bins) {
		for (int i = 0; i < bin_count; i++) {
			if (filter_bins[i]) {
				free(filter_bins[i]);
			}
		}
		free(filter_bins);
	}
	if (status != AEROSPIKE_OK || err.code != AEROSPIKE_OK) {
		if (status == AEROSPIKE_OK) {
			RETURN_LONG(err.code);
		}
		RETURN_LONG(status);
	}
	RETURN_LONG(AEROSPIKE_OK);
}
