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

#include "php_aerospike_types.h"
#include "php.h"
#include "aerospike/as_error.h"
#include "aerospike/as_status.h"

AerospikeClient* get_aerospike_from_zobj(zend_object* zval_wrapper) {
	return (AerospikeClient*)((char *)zval_wrapper - XtOffsetOf(AerospikeClient, zobj));
}

void update_client_error(zval* client_obj, int code, const char* msg, bool in_doubt) {
	if (!client_obj || Z_TYPE_P(client_obj) != IS_OBJECT) {
		return;
	}

	AerospikeClient* php_client = get_aerospike_from_zobj(Z_OBJ_P(client_obj));
	if (php_client) {
		as_error_set_message(&(php_client->client_error), code, msg);
		php_client->client_error.in_doubt = in_doubt;
	}
}

void reset_client_error(zval* client_obj) {
	if (!client_obj || Z_TYPE_P(client_obj) != IS_OBJECT) {
		return;
	}
	AerospikeClient* php_client = get_aerospike_from_zobj(Z_OBJ_P(client_obj));
	if (php_client) {
		as_error_reset(&(php_client->client_error));
	}
}

as_status check_object_and_connection(zval* aerospike_container, as_error* err) {
	if (!aerospike_container) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Invalid class object");
		return AEROSPIKE_ERR_CLIENT;
	}
	AerospikeClient* php_client = get_aerospike_from_zobj(Z_OBJ_P(aerospike_container));

	if (!php_client || ! php_client->as_client || !php_client->is_valid) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Invalid class object");
		return AEROSPIKE_ERR_CLIENT ;
	}

	if (!php_client->is_connected) {
		as_error_update(err, AEROSPIKE_ERR_CLUSTER, "Not connected");
		return AEROSPIKE_ERR_CLUSTER;
	}
	return AEROSPIKE_OK;
}

void set_policy_defaults_from_ini(as_config* config, AerospikeClient* client) {
	char* lua_user_path_str = NULL;
	int int_ini_value;

	config->conn_timeout_ms = INI_INT("aerospike.connect_timeout");

	int_ini_value = INI_INT("aerospike.read_timeout");
	config->policies.read.base.total_timeout = int_ini_value;
	config->policies.info.timeout = int_ini_value;
	config->policies.batch.base.total_timeout = int_ini_value;

	int_ini_value = INI_INT("aerospike.write_timeout");
	config->policies.write.base.total_timeout = int_ini_value;
	config->policies.operate.base.total_timeout = int_ini_value;
	config->policies.remove.base.total_timeout = int_ini_value;
	config->policies.apply.base.total_timeout = int_ini_value;

	int_ini_value = INI_INT("aerospike.key_policy");
	config->policies.read.key = (as_policy_key)int_ini_value;
	config->policies.write.key = (as_policy_key)int_ini_value;
	config->policies.operate.key = (as_policy_key)int_ini_value;
	config->policies.remove.key = (as_policy_key)int_ini_value;

	int_ini_value = INI_INT("aerospike.compression_threshold");
	config->policies.write.compression_threshold = int_ini_value;

	int_ini_value = INI_INT("aerospike.key_gen");
	config->policies.write.gen = (as_policy_gen)int_ini_value;
	config->policies.operate.gen = (as_policy_gen)int_ini_value;
	config->policies.apply.gen = (as_policy_gen)int_ini_value;
	config->policies.remove.gen = (as_policy_gen)int_ini_value;

	int_ini_value = INI_INT("aerospike.serializer");
	client->serializer_type = int_ini_value;

	lua_user_path_str = INI_STR("aerospike.udf.lua_user_path");
	if (lua_user_path_str && strlen(lua_user_path_str) < AS_CONFIG_PATH_MAX_SIZE) {
		strcpy(config->lua.user_path, lua_user_path_str);
	}
}
