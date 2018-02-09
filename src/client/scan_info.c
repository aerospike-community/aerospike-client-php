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
#include "aerospike/aerospike_scan.h"
#include "policy_conversions.h"

#define SI_PROGRESS_KEY "progress_pct"
#define SI_RECORDS_KEY "records_read"
#define SI_STATUS_KEY "status"


/* {{{ proto int Aerospike::scanInfo ( int scan_id, array &info [, array $options ] )
    Gets the status of a background scan triggered by scanApply()  */
PHP_METHOD(Aerospike, scanInfo) {

	as_error err;
	as_error_init(&err);
	as_scan_info ret_scan_info;

	as_policy_info info_policy;
	as_policy_info* info_policy_p = NULL;
	zval* z_policy = NULL;

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	zend_long scan_id;
	zval* z_scan_info = NULL;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz/|z",
			&scan_id, &z_scan_info, &z_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to scan apply.", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	zval_dtor(z_scan_info);
	ZVAL_NULL(z_scan_info);

	if (zval_to_as_policy_info(z_policy, &info_policy, &info_policy_p,
			&as_client->config.policies.info) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy.", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	info_policy_p = &info_policy;

	if(aerospike_scan_info(as_client, &err, info_policy_p, (uint64_t)scan_id, &ret_scan_info) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}

	array_init(z_scan_info);
	add_assoc_long(z_scan_info, SI_PROGRESS_KEY, ret_scan_info.progress_pct);
	add_assoc_long(z_scan_info, SI_RECORDS_KEY, ret_scan_info.records_scanned);
	add_assoc_long(z_scan_info, SI_STATUS_KEY, ret_scan_info.status);

	RETURN_LONG(err.code);

}
/* }}} */
