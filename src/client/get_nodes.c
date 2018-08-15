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
#include "aerospike/as_cluster.h"
#include "php_aerospike_types.h"


/* {{{ proto array Aerospike::getNodes( void )
    Gets the host information of the cluster nodes */
PHP_METHOD(Aerospike, getNodes)
{
	as_error err;
	as_error_init(&err);
	reset_client_error(getThis());

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_nodes* nodes = NULL;
	as_node* node = NULL;
	as_cluster* cluster = NULL;
	zval ip_pair;
	int host_length;
	char* split_point = NULL;
	char* hostname = NULL;


	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	cluster = as_client->cluster;
	if (!cluster) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLUSTER, "Invalid cluster", false);
		RETURN_LONG(AEROSPIKE_ERR_CLUSTER);
	}

	nodes = as_nodes_reserve(cluster);

	array_init(return_value);

	for (int i = 0; i < nodes->size; i++) {
		node = nodes->array[i];
		hostname = (char*)as_node_get_address_string(node);

		split_point = strrchr(hostname, ':');
		if (!split_point) {
			goto CLEANUP;
		}

		host_length = split_point - hostname;

		array_init(&ip_pair);
		add_assoc_stringl(&ip_pair, "addr", hostname, host_length);
		add_assoc_string(&ip_pair, "port", split_point + 1);
		/* Push the ["host", "port"] to the return array */
		add_next_index_zval(return_value, &ip_pair);
	}

CLEANUP:
	as_nodes_release(nodes);
}
/* }}} */
