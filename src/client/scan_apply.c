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
#include "aerospike/aerospike_scan.h"
#include "policy_conversions.h"
#include "php.h"

/* {{{ proto int Aerospike::scanApply( string ns, string set, string module, string function, array args, int &scan_id [, array options ] )
    Applies a record UDF to each record of a set using a background scan  */
PHP_METHOD(Aerospike, scanApply) {
	as_error err;
	as_error_init(&err);

	char* ns = NULL;
	char* set = NULL;
	char* module = NULL;
	char* function = NULL;
	size_t ns_len, set_len, module_len, function_len;
	HashTable* args_list = NULL;

	zval* job_id = NULL; // Reference to be filled with the scan's id
	as_policy_scan scan_policy;
	as_policy_scan* scan_policy_p = NULL;
	zval* z_policy = NULL;

	as_scan user_scan;
	as_list* as_args_list = NULL;

	bool scan_initialized = false;

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	uint64_t scan_id = 0;

	int serializer_type = INI_INT("aerospike.serializer");

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sssshz/|z",
			&ns, &ns_len, &set, &set_len,
			&module, &module_len,
			&function, &function_len,
			&args_list, &job_id, &z_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to scanApply", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	// Free the old value of the parameter by reference
	zval_dtor(job_id);
	ZVAL_NULL(job_id);

	if (ns_len == 0) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Empty Namespace", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	if (module_len == 0 || function_len == 0) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Module and function must not be empty", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_scan(z_policy, &scan_policy,
			&scan_policy_p, &as_client->config.policies.scan) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid scan policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	as_status status = set_scan_options_from_policy_hash(&user_scan, z_policy);

	if (status != AEROSPIKE_OK) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid scan options");
		goto CLEANUP;
	}

	if (z_hashtable_to_as_list(args_list, &as_args_list, &err, serializer_type) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (!as_scan_init(&user_scan, ns, set))
	{
		as_list_destroy(as_args_list);
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Unable to create scan");
		goto CLEANUP;
	}

	scan_initialized = true;
	if (!as_scan_apply_each(&user_scan, module, function, as_args_list)) {
		as_list_destroy(as_args_list);
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Failed to add UDF to scan");
		goto CLEANUP;
	}

	if (aerospike_scan_background(as_client, &err, scan_policy_p, &user_scan, &scan_id) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	// 0 for timeout forces the default to be used
	if (aerospike_scan_wait(as_client, &err, NULL, scan_id, 0) == AEROSPIKE_OK) {
		ZVAL_LONG(job_id, scan_id);
	}

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
