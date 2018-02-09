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

#include "conversions.h"
#include "aerospike/as_error.h"
#include "aerospike_class.h"
#include "php_aerospike_types.h"
#include "user_callbacks.h"
#include "aerospike/aerospike_scan.h"
#include "policy_conversions.h"


/* {{{ proto int Aerospike::scan( string ns, string set, callback record_cb [, array select [, array options ]] )
    Returns all the records in a set to a callback method  */
PHP_METHOD(Aerospike, scan) {
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;
	as_error_init(&err);
	zend_fcall_info callback_info;
	zend_fcall_info_cache callback_cache;
	char* set = NULL;
	char* ns = NULL;
	size_t set_len = 0;
	size_t ns_len = 0;
	HashTable* select_bins = NULL;
	uint32_t select_count = 0;
	user_callback_function callback_function_data;
	zval* z_policy = NULL; //Figure this out need as_policy_scan converter
	as_policy_scan scan_policy;
	as_policy_scan* scan_policy_p = NULL;
	bool scan_initialized = false;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssf|h!z",
			&ns, &ns_len, &set, &set_len, &callback_info, &callback_cache,
			&select_bins, &z_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to scan", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	if (!ns_len) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid namespace", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_scan(z_policy, &scan_policy,
			&scan_policy_p, &as_client->config.policies.scan) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid scan policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	scan_policy_p = &scan_policy;


	callback_info.retval = NULL;
	callback_function_data.err = &err;
	callback_function_data.callback = callback_info;
	callback_function_data.callback_cache = callback_cache;
	callback_function_data.cb_mutex = &AEROSPIKE_G(query_cb_mutex);

	as_scan user_scan;
	as_scan_init(&user_scan, ns, set);
	scan_initialized = true;

	as_status status = set_scan_options_from_policy_hash(&user_scan, z_policy);

	if (status != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid scan options");
		goto CLEANUP;
	}

	if (select_bins) {
		select_count = zend_hash_num_elements(select_bins);
		if (select_count > 0) {
			as_scan_select_inita(&user_scan, select_count);

			zval* entry = NULL;

			ZEND_HASH_FOREACH_VAL(select_bins, entry)
			{
				if (Z_TYPE_P(entry) != IS_STRING) {
					as_error_update(&err, AEROSPIKE_ERR_PARAM, "Bin names must be strings");
					goto CLEANUP;
				}
				//TODO Validate bin length?
				as_scan_select(&user_scan, Z_STRVAL_P(entry));
			} ZEND_HASH_FOREACH_END();
		}
	}

	aerospike_scan_foreach(as_client, &err, scan_policy_p, &user_scan, user_callback_wrapper, (void*)&callback_function_data);

CLEANUP:
	if (scan_initialized) {
		as_scan_destroy(&user_scan);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */
