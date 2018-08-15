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

#include "php.h"
#include "aerospike_class.h"
#include "policy_conversions.h"
#include "php_aerospike_types.h"

#include "aerospike/aerospike_index.h"


/* {{{ proto int Aerospike::addIndex( string ns, string set, string bin, string name,
 * int index_type, int data_type [, array options ] )
 Creates a secondary index on a bin of a specified set */
PHP_METHOD(Aerospike, addIndex) {
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;
	char* ns = NULL;
	char* set = NULL;
	char* bin_name = NULL;
	char* index_name = NULL;
	size_t ns_len, set_len, bin_len, index_len;
	zend_long index_type, data_type;
	zval* z_index_policy = NULL;
	as_policy_info index_policy;
	as_policy_info* index_policy_p = NULL;
	as_index_task task;

	as_error_init(&err);
	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssssll|z",
			&ns, &ns_len, &set, &set_len, &bin_name, &bin_len,
			&index_name, &index_len, &index_type, &data_type,
			&z_index_policy) != SUCCESS) {
				update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to addIndex", false);
				RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_info(z_index_policy, & index_policy,
			&index_policy_p, &as_client->config.policies.info) == AEROSPIKE_OK) {
		index_policy_p = &index_policy;
	} else {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid info policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	if (bin_len > AS_BIN_NAME_MAX_LEN) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Bin name is too long", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (aerospike_index_create_complex(
			as_client, &err, &task, index_policy_p, ns, set, bin_name, index_name, index_type, data_type) !=
			AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}
	//Wait for the index to be created

	if (aerospike_index_create_wait(&err, &task, 2000) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::dropIndex( string ns, string name [, array options ] )
    Drops a secondary index from a specified set */
PHP_METHOD(Aerospike, dropIndex) {
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;
	char* ns = NULL;
	char* index_name = NULL;
	size_t ns_len, index_len;
	zval* z_index_policy = NULL;
	as_policy_info index_policy;
	as_policy_info* index_policy_p = NULL;

	as_error_init(&err);
	reset_client_error(getThis());



	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|z", &ns, &ns_len, &index_name, &index_len, &z_index_policy) !=
			SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to dropIndex", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	if (zval_to_as_policy_info(z_index_policy, & index_policy,
			&index_policy_p, &as_client->config.policies.info) == AEROSPIKE_OK) {
		index_policy_p = &index_policy;
	} else {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid info policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	if (aerospike_index_remove(as_client, &err, index_policy_p, ns, index_name) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */
