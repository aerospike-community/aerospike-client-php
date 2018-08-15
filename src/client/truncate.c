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

/* {{{ proto int Aerospike::truncate ( string ns, string set, long nanos, [, array policy ] )
    truncate items in a namespace and optionally scoped to a set */
PHP_METHOD(Aerospike, truncate)
{

	as_error err;
	as_error_init(&err);

	char* ns = NULL;
	char* set = NULL;

	size_t ns_len;
	size_t set_len;

	zend_long nanos;
	uint64_t nanos64;

	as_policy_info info_policy;
	as_policy_info* info_policy_p = NULL;
	zval* z_info_policy = NULL;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_client = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss!l|z",
							  &ns, &ns_len, &set, &set_len, &nanos, &z_info_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to truncate", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (nanos < 0) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Nanos arg must not be negative", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	nanos64 = (uint64_t)nanos;

	if (ns_len == 0) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Namespace must not be empty", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (ns_len >= AS_NAMESPACE_MAX_SIZE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Namespace too long", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (set && set_len >= AS_SET_MAX_SIZE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Set too long", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_info(z_info_policy, &info_policy, &info_policy_p, &as_client->config.policies.info)
			!= AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	aerospike_truncate(as_client, &err, info_policy_p, ns, set, nanos64);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code)
}
/* }}} */
