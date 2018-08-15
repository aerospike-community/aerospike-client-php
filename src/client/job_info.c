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
#include "php_aerospike_types.h"
#include "aerospike/as_job.h"
#include "policy_conversions.h"

#define JOB_I_PROGRESS_KEY "progress_pct"
#define JOB_I_RECORDS_KEY "records_read"
#define JOB_I_STATUS_KEY "status"

#define JOB_INFO_QUERY "query"
#define JOB_INFO_SCAN "scan"

/* {{{ proto int Aerospike::jobInfo ( integer $job_id, string $module, array &$info [, array $options ] )
 * module should be Aerospike::JOB_QUERY or Aerospike::JOB_SCAN
*/
PHP_METHOD(Aerospike, jobInfo) {

	as_error err;
	as_error_init(&err);
	as_job_info ret_job_info;

	as_policy_info info_policy;
	as_policy_info* info_policy_p = NULL;
	zval* z_policy = NULL;

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	zend_long job_id;
	zval* z_job_info = NULL;
	char* module = NULL;
	size_t module_len;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lsz/|z",
			&job_id, &module, &module_len,
			&z_job_info, &z_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to job_info.", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	zval_dtor(z_job_info);
	ZVAL_NULL(z_job_info);

	if (strcmp(module, JOB_INFO_QUERY) && strcmp(module, JOB_INFO_SCAN)) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Module argument must be query or scan", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_info(z_policy, &info_policy,
							   &info_policy_p, &as_client->config.policies.info) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy.", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	info_policy_p = &info_policy;

	if (aerospike_job_info(as_client, &err, info_policy_p,
			module, (uint64_t)job_id, false, &ret_job_info) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	array_init(z_job_info);
	add_assoc_long(z_job_info, JOB_I_PROGRESS_KEY, ret_job_info.progress_pct);
	add_assoc_long(z_job_info, JOB_I_RECORDS_KEY, ret_job_info.records_read);
	add_assoc_long(z_job_info, JOB_I_STATUS_KEY, ret_job_info.status);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);

}
/* }}} */

