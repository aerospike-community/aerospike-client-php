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
#include "aerospike/as_node.h"
#include "aerospike/aerospike_info.h"

typedef struct _info_each_cb_data {
	as_error* err;
	zval* response_array;
	HashTable* filter_hosts;
}info_each_cb_data;

static inline bool node_in_hosts_list(const as_node* node, HashTable* hosts_ary);
static bool is_valid_hosts_list(HashTable* hosts_ary);

static bool AerospikeClient_Info_each(as_error * err, const as_node * node,
		const char * req, char * res, void * udata);


/* {{{ proto int Aerospike::info( string request, string &response [, array host [, array options ]] )
    Sends an info command to a cluster node */
PHP_METHOD(Aerospike, info)
{
	as_error err;
	as_error_init(&err);
	reset_client_error(getThis());

	char* request = NULL;
	size_t request_len;
	zval* response_zval = NULL;
	zval* z_policy = NULL;

	as_host* host = NULL;
	char* host_str = NULL;
	uint16_t port_number;

	char* response_str = NULL;
	HashTable* destination_host = NULL;
	zval* host_zval = NULL;
	zval* port_zval = NULL;

	as_policy_info info_policy;
	as_policy_info* info_policy_p = NULL;
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	host = (as_host *)as_vector_get(as_client->config.hosts, 0);
	host_str = host->name;
	port_number = host->port;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/|h!z",
			&request, &request_len,
			&response_zval, &destination_host,
			&z_policy) == FAILURE) {
		update_client_error(getThis(),AEROSPIKE_ERR_PARAM, "Invalid arguments to info", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	/* Cleanup any thing held by the return parameter */
	zval_dtor(response_zval);
	ZVAL_NULL(response_zval);

	if (z_policy){
		if (zval_to_as_policy_info(z_policy, &info_policy,
				&info_policy_p, &as_client->config.policies.info) != AEROSPIKE_OK) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy.", false);
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
		}
		info_policy_p = &info_policy;
	}

//	addr hostname or IP of the node
//	port
	/* This should be of the form ['addr'=>'hostname', 'port'=>3000] */
	if (destination_host) {
		host_zval = zend_hash_str_find(destination_host, "addr", strlen("addr"));
		if (!host_zval) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Host entry must contain an addr");
			goto CLEANUP;
		}
		if (Z_TYPE_P(host_zval) != IS_STRING) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "addr entry must be a string");
			goto CLEANUP;
		}
		host_str = Z_STRVAL_P(host_zval);

		port_zval = zend_hash_str_find(destination_host, "port", strlen("port"));
		if (!port_zval) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Host entry must contain a port");
			goto CLEANUP;
		}
		if (Z_TYPE_P(port_zval) != IS_LONG) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "port entry must be an integer");
			goto CLEANUP;
		}
		port_number = (uint16_t)Z_LVAL_P(port_zval);
	}

	if (aerospike_info_host(as_client, &err, info_policy_p, host_str, port_number, request, &response_str)
			!= AEROSPIKE_OK) {
		goto CLEANUP;
	}
	/* It is possible that an empty response will be returned, so only setup the return
	 * if we get one back.
	 */
	if (response_str) {
		ZVAL_STRING(response_zval, response_str);
	}

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (response_str) {
		free(response_str);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto array Aerospike::infoMany( string request, [, array config [, array options ]] )
    Sends an info command to several or all cluster nodes */
PHP_METHOD(Aerospike, infoMany) {

	as_error err;
	as_error_init(&err);
	reset_client_error(getThis());

	char* request = NULL;
	size_t request_len;
	zval* z_policy = NULL;
	HashTable* filter_hosts = NULL;
	zval* cb_filter_zval = NULL;
	HashTable* cb_filter_hosts = NULL;

	as_policy_info info_policy;
	as_policy_info* info_policy_p = NULL;
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	info_each_cb_data cb_data;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_NULL();
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;


	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|h!z", &request,
			&request_len, &filter_hosts, &z_policy ) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters for infoMany.", false);
		RETURN_NULL();
	}

	if (filter_hosts) {
		if (!is_valid_hosts_list(filter_hosts)) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid list of hosts.", false);
			RETURN_NULL();
		}
		cb_filter_zval = zend_hash_str_find(filter_hosts, "hosts", strlen("hosts"));
		if (!cb_filter_zval || Z_TYPE_P(cb_filter_zval) != IS_ARRAY) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid list of hosts.", false);
			RETURN_NULL();
		}
		cb_filter_hosts = Z_ARRVAL_P(cb_filter_zval);
	}

	if (z_policy){
		if (zval_to_as_policy_info(z_policy, &info_policy,
				&info_policy_p, &as_client->config.policies.info) != AEROSPIKE_OK) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy for infoMany.", false);
			RETURN_NULL();
		}
		info_policy_p = &info_policy;
	}

	array_init(return_value);
	cb_data.err = &err;
	cb_data.response_array = return_value;
	cb_data.filter_hosts = cb_filter_hosts;

	aerospike_info_foreach(as_client, &err, info_policy_p, request,
			(aerospike_info_foreach_callback)AerospikeClient_Info_each, (void*)&cb_data);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		zval_dtor(return_value);
		RETURN_NULL();
	}
	return;
}
/* }}} */


/* Callback function used for infoMany. Adds the response to an array, if the host info
 * is contained in the filter.
 */
static bool AerospikeClient_Info_each(as_error * err, const as_node * node,
		const char * req, char * res, void * udata) {

	bool should_include;
	info_each_cb_data* cb_data = (info_each_cb_data*)udata;
	if (!node) {
		as_error_update(cb_data->err, AEROSPIKE_ERR_CLIENT, "Null node passed in infoMany");
		return false;
	}

	/* We add the response if there are no hosts to select responses from, or if the list is provided
	 * and this node is one of them
	 */
	should_include = cb_data->filter_hosts ? node_in_hosts_list(node, cb_data->filter_hosts) : true;

	if (should_include) {
		if (res) {
			add_assoc_string(cb_data->response_array, node->name, res);
		} else {
			add_assoc_null(cb_data->response_array, node->name);
		}
	}
	return true;
}


/*
 * True if the argument is an array of host entries, else false
 * should look like ["hosts"=>[[addr, port]]]
 */
static bool is_valid_hosts_list(HashTable* outer_hosts_ary) {
	HashTable* host_pair = NULL;
	zval* hostname = NULL;
	zval* port = NULL;
	zval* z_host_pair = NULL;

	HashTable* hosts_ary = NULL;
	zval* z_hosts_ary = NULL;
	z_hosts_ary = zend_hash_str_find(outer_hosts_ary, "hosts", strlen("hosts"));
	if (!z_hosts_ary || Z_TYPE_P(z_hosts_ary) != IS_ARRAY) {
		return false;
	}
	hosts_ary = Z_ARRVAL_P(z_hosts_ary);

	/* Each entry should be of the form: ["addr"=> "192.168.1.1", "port"=>4500] *
	 *  return false if one does not match the expected format
	 */
	ZEND_HASH_FOREACH_VAL(hosts_ary, z_host_pair) {
		if (!z_host_pair || Z_TYPE_P(z_host_pair) != IS_ARRAY) {
			return false;
		}
		host_pair = Z_ARRVAL_P(z_host_pair);

		if (zend_hash_num_elements(host_pair) != 2) {
			return false;
		}

		hostname = zend_hash_str_find(host_pair, "addr", strlen("addr"));
		port = zend_hash_str_find(host_pair, "port", strlen("port"));

		if (!hostname || !port) {
			return false;
		}

		if (Z_TYPE_P(hostname) != IS_STRING) {
			return false;
		}

		if (Z_TYPE_P(port) != IS_LONG) {
			return false;
		}
	}ZEND_HASH_FOREACH_END();

	return true;
}


/* Check if a node* pointer is in a php array of host pairs
 * The hosts array will have been validated at this point, so we don't check bounds
 * on any of the individual elements */
static inline bool node_in_hosts_list(const as_node* node, HashTable* hosts_ary) {
	zval* z_host_pair = NULL;
	char host_buffer[AS_HOSTNAME_SIZE] = {0};
	zval* hostname = NULL;
	zval* port = NULL;
	HashTable* host_pair = NULL;

	ZEND_HASH_FOREACH_VAL(hosts_ary, z_host_pair) {
		host_pair = Z_ARRVAL_P(z_host_pair);
		hostname = zend_hash_str_find(host_pair, "addr", strlen("addr"));
		port = zend_hash_str_find(host_pair, "port", strlen("port"));

		snprintf(host_buffer, AS_HOSTNAME_SIZE, "%s:%ld", Z_STRVAL_P(hostname), Z_LVAL_P(port));
		if (!strcmp(host_buffer, as_node_get_address_string((as_node*)node))) {
			return true;
		}
	}ZEND_HASH_FOREACH_END();

	return false;
}
