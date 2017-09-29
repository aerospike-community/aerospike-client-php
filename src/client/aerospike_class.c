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

/* Aerospike_class.c */

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_aerospike.h"

#include "conversions.h"
#include "aerospike/aerospike.h"
#include "aerospike_class.h"
#include "php_aerospike_types.h"
#include "register_policy_constants.h"
#include "register_constants.h"
#include "logging.h"
#include "persistent_list.h"
#include "zend_exceptions.h"
#include <stdbool.h>
#include "aerospike_session.h"
// SETUP FUNCTIONS



static void set_as_config_from_ini(as_config* config);
static as_status set_as_config(as_config* config, HashTable* z_conf_hash);
static as_status set_policy_defaults_from_hash(as_config* config, AerospikeClient* client, HashTable* policy_hash);
static as_status add_hosts_from_zhash(as_config* config, HashTable* z_hosts);

static void aerospike_object_destructor(zend_object *object);
static void aerospike_object_free_storage(zend_object *object);

/* Actually global variables here*/
zend_class_entry     *aerospike_ce;
zend_object_handlers aerospike_ce_handlers;
static zend_function_entry Aerospike_class_functions[] =
{
    /*
     ********************************************************************
     *  Client Object APIs:
     ********************************************************************
     */
	PHP_ME(Aerospike, __construct, construct_arg_info, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, addIndex, add_index_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, apply, apply_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, append, append_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, close, close_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, deregister, deregister_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, dropIndex, drop_index_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, error, error_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, errorno, errorno_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, exists, exists_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, get, get_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, getKeyDigest, getKeyDigest_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, getRegistered, get_registered_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, increment, increment_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, initKey, initkey_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, isConnected, is_connected_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, jobInfo, job_info_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listAppend, list_append_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listClear, list_clear_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listGet, list_get_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listGetRange, list_get_range_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listInsert, list_insert_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listInsertItems, list_insert_items_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listMerge, list_merge_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listPop, list_pop_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listPopRange, list_pop_range_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listRemove, list_remove_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listRemoveRange, list_remove_range_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listSize, list_size_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listSet, list_set_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listTrim, list_trim_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, listRegistered, list_registered_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, operate, operate_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, operateOrdered, operate_ordered_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, put, put_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, prepend, prepend_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, reconnect, reconnect_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, register, register_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, remove, remove_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, removeBin, remove_bin_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, select, select_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, setDeserializer, set_deserializer_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, setSerializer, set_serializer_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, shmKey, shm_key_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, truncate, truncate_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, touch, touch_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, predicateEquals, predicate_equals_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateGeoContainsGeoJSONPoint, predicate_geo_contains_json_point_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateGeoContainsPoint, predicate_geo_contains_point_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateGeoWithinRadius, predicate_geo_within_radius_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateGeoWithinGeoJSONRegion, predicate_geo_within_region_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateRange, predicate_range_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateBetween, predicate_between_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, predicateContains, predicate_contains_arg_info, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Aerospike, setLogLevel, set_log_level_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, setLogHandler, set_log_handler_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, scan, scan_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, scanApply, scan_apply_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, scanInfo, scan_info_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, query, query_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, queryApply, query_apply_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, aggregate, aggregate_arg_info, ZEND_ACC_PUBLIC)
	/* Info Methods */
	PHP_ME(Aerospike, info, info_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, getNodes, get_nodes_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, infoMany, info_many_arg_info, ZEND_ACC_PUBLIC)
	/* Batch Methods */
	PHP_ME(Aerospike, existsMany, exists_many_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, getMany, get_many_arg_info, ZEND_ACC_PUBLIC)
	/* Security Methods */
	PHP_ME(Aerospike, changePassword, change_password_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, setPassword, set_password_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, createRole, create_role_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, createUser, create_user_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, dropRole, drop_role_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, grantRoles, grant_roles_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, revokeRoles, revoke_roles_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, grantPrivileges, grant_privileges_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, revokePrivileges, revoke_privileges_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, queryRole, query_role_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, queryRoles, query_roles_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, queryUser, query_user_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, queryUsers, query_users_arg_info, ZEND_ACC_PUBLIC)
	PHP_ME(Aerospike, dropUser, drop_user_arg_info, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/*
Actually load all of the aerospike information
*/

bool register_aerospike_class(void)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Aerospike", Aerospike_class_functions);
	aerospike_ce = zend_register_internal_class(&ce);

	aerospike_ce->create_object = aerospike_create_object;
	memcpy(&aerospike_ce_handlers, zend_get_std_object_handlers(), sizeof(aerospike_ce_handlers));

	aerospike_ce_handlers.free_obj = aerospike_object_free_storage;
	aerospike_ce_handlers.dtor_obj = aerospike_object_destructor;
	aerospike_ce_handlers.offset = XtOffsetOf(AerospikeClient, zobj);

	// Register all status codes for the class
	register_aerospike_status_codes(aerospike_ce);
	register_aerospike_class_constants(aerospike_ce);
	register_aerospike_option_constants(aerospike_ce);
	register_logger_constants(aerospike_ce);

	return true;

}

static void aerospike_object_free_storage(zend_object *object TSRMLS_DC) {
	AerospikeClient* client = get_aerospike_from_zobj(object);
	as_error err;
	/* It is only safe to destroy the client if it has been fully constructed
	 * and is not being shared */
	if (client->is_valid && !client->is_persistent) {
		/* This was likely called by the destructor,
		but it is safe to call it again */
		aerospike_close(client->as_client, &err);
		aerospike_destroy(client->as_client);
	}
	zend_object_std_dtor(object);
 	return;
}

static void aerospike_object_destructor(zend_object *object TSRMLS_DC) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* client = get_aerospike_from_zobj(object);
	if (!client->is_persistent) {
		/* If this close fails, it will be run again in free storage */
		aerospike_close(client->as_client, &err);
	}
 	return;
}

zend_object *aerospike_create_object(zend_class_entry *ce)
{
    AerospikeClient* aerospike_obj;

    aerospike_obj = ecalloc(1, sizeof(*aerospike_obj) + zend_object_properties_size(ce));
    aerospike_obj->as_client = NULL;

    zend_object_std_init(&aerospike_obj->zobj, ce);
    object_properties_init(&aerospike_obj->zobj, ce);

    aerospike_obj->zobj.handlers = &aerospike_ce_handlers;

    return &aerospike_obj->zobj;
}


/* {{{ proto Aerospike::__construct(array config [, bool persistent_connection=true [, array options]]))
 *
 * Constructor For the Aerospike class, takes a config object and creates the connection to the cluster via
   creating the aerospike* c struct;
	construct([=>]config, bool persistent, hash options)
 */
PHP_METHOD(Aerospike, __construct)
{
	as_config config;
	as_error err;
	as_status status = AEROSPIKE_OK;
	// Parameter values
	HashTable* z_config = NULL;
	HashTable* z_options = NULL;
	zend_bool persistent = true;

	zval* z_hosts = NULL;

	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	client->is_connected = false;
	client->is_valid = false;
	client->is_persistent = true;
	client->serializer_type = INI_INT("aerospike.serializer");
	as_error_init(&client->client_error);
	zend_error_handling error_handling; // store the old error handling here

	zend_update_property_long(aerospike_ce, getThis(), "errorno", sizeof("errorno") - 1, AEROSPIKE_OK);
	zend_update_property_string(aerospike_ce, getThis(), "error", sizeof("error") - 1, "");

	zend_replace_error_handling(EH_THROW, NULL, &error_handling TSRMLS_CC); // fail to construct here must raise error
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "h|bh",
		&z_config, &persistent, &z_options) != SUCCESS) {
		zend_restore_error_handling(&error_handling);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zend_restore_error_handling(&error_handling);

	if (!client) {
		zend_throw_exception(NULL, "Unable to create Aerospike object", 0);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}

	z_hosts = zend_hash_str_find(z_config, "hosts", strlen("hosts"));
	if (!z_hosts) {
		zend_throw_exception(NULL, "An entry for hosts must be included in the config array", 0);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	} 

	if (Z_TYPE_P(z_hosts) != IS_ARRAY) {
		zend_throw_exception(NULL, "hosts entry must be an array", 0);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	as_error_init(&err);
	as_config_init(&config);

	/*Parallel threads on scan/query cause issues in ZTS mode */
#ifdef ZTS
	config.thread_pool_size = 0;
#endif


	/* Add each of the host entries to the as_config.hosts struct */
	status = add_hosts_from_zhash(&config, Z_ARRVAL_P(z_hosts));
	if (status != AEROSPIKE_OK) {
		RETURN_LONG(status);
	}

	set_policy_defaults_from_ini(&config, client);
	set_as_config_from_ini(&config);

	if (set_as_config(&config, z_config) != AEROSPIKE_OK) {
		zend_throw_exception(NULL, "Invalid configuration values", 0);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	// set_policy_defaults from ini
	if (set_policy_defaults_from_hash(&config, client, z_options) != AEROSPIKE_OK) {
		zend_throw_exception(NULL, "Invalid policy values", 0);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	client->as_client = aerospike_new(&config);
	if (!client->as_client) {
		zend_throw_exception(NULL, "Failed to create aerospike object", 0);
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	client->is_valid = true;
	/* Handle use of persistent connection*/
	if (persistent) {
		client->is_persistent = true;
		status = persistent_connect(client);
		if (status != AEROSPIKE_OK) {
			zend_throw_exception(NULL, "Failed to connect", 0);
			RETURN_LONG(status);
		}
	} else {
		client->is_persistent = false;
		status = non_persistent_connect(client);
		if (status != AEROSPIKE_OK) {
			zend_throw_exception(NULL, "Failed to connect", 0);
			RETURN_LONG(status);
		}
	}
	client->is_connected = true;

    RETURN_LONG(0);
}
/* }}} */


/* {{{ proto array Aerospike::initKey( string ns, string set, int|string pk [, bool digest=false ])
    Helper which builds the key array that is needed for read/write operations */
PHP_METHOD(Aerospike, initKey)
{
	zend_bool is_digest = false; //zend_bool is unsigned char
	char* namespace = NULL;
	char* set = NULL;
	size_t namespace_len, set_len;
	zval* primary_key = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssz|b",
		&namespace, &namespace_len, &set, &set_len, &primary_key, &is_digest) != SUCCESS
		) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments for initKey");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	reset_client_error(getThis());
	array_init(return_value);

	add_assoc_stringl(return_value, NAMESPACE_KEY, namespace, namespace_len);
	add_assoc_stringl(return_value, SET_KEY, set, set_len);

	switch(Z_TYPE_P(primary_key)) {
		case(IS_STRING):
			if (is_digest) {
				if (Z_STRLEN_P(primary_key) != AS_DIGEST_VALUE_SIZE) {
					update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Digest Must be 20 bytes");
					zval_dtor(return_value);
					RETURN_NULL();
				}
				add_assoc_stringl(return_value, DIGEST_KEY, Z_STRVAL_P(primary_key), Z_STRLEN_P(primary_key));
			}	
			else {
				add_assoc_stringl(return_value, PK_KEY, Z_STRVAL_P(primary_key), Z_STRLEN_P(primary_key));
			}
			break;
		case(IS_LONG):
			// For some reason the current client lets you specify a long as a digest, error for now
			if (is_digest) {
				zval_dtor(return_value);
				RETURN_NULL();
			}
			add_assoc_long(return_value, PK_KEY, Z_LVAL_P(primary_key));
			break;
		// case bytes
		default:
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid type for primary key");
			zval_dtor(return_value);
			RETURN_NULL();
	}
}
/* }}} */


/* {{{ proto string Aerospike::error ( void )
    Displays the error message associated with the last operation */
PHP_METHOD(Aerospike, error)
{
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	if (client) {
		RETURN_STRING(client->client_error.message);
	} else {
		RETURN_STRING("");
	}
}
/* }}} */


/* {{{ proto int Aerospike::errorno ( void )
   Displays the status code associated with the last operation */
PHP_METHOD(Aerospike, errorno)
{
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	if (client) {
		RETURN_LONG(client->client_error.code);
	}
}
/* }}} */


/* {{{ proto bool Aerospike::isConnected( void )
   Tests whether the connection to the cluster was established */
PHP_METHOD(Aerospike, isConnected) {
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = NULL;

	if (client && client->as_client) {
		as_ptr = client->as_client;
	} else {
		// THIS IS An unrecoverable error
		RETURN_FALSE;
	}
	bool connected = false;
	connected = (client->is_connected && aerospike_cluster_is_connected(as_ptr));

	if (connected) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */


/* {{{ proto Aerospike::reconnect( void )
    Closes then reopens all connections to the cluster */
PHP_METHOD(Aerospike, reconnect) {
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = NULL;
	as_error err;
	as_error_init(&err);

	reset_client_error(getThis());

	if (client && client->as_client) {
		as_ptr = client->as_client;
	} else {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Client object is invalid");
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}

	if (client->is_connected) {
		update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "Client already connected");
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}

	aerospike_connect(as_ptr, &err);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
		RETURN_LONG(err.code);
	} else {
		client->is_connected = true;
	}

	RETURN_LONG(AEROSPIKE_OK);
}
/* }}} */


/* {{{ proto array Aerospike::shm_key( void )
    Function which returns the shm_key which is set and returns NULL if shm is not configured */
PHP_METHOD(Aerospike, shmKey) {
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = NULL;

	if (client && client->as_client) {
		as_ptr = client->as_client;
	} else {
		// THIS IS An unrecoverable error
		RETURN_NULL();
	}
	if (as_ptr->config.use_shm) {
		RETURN_LONG(as_ptr->config.shm_key);
	} else {
		RETURN_NULL();
	}
}
/* }}} */


/* {{{ proto Aerospike::close( void )
    Closes the client's connections to the cluster. No-op if the connection is shared */
PHP_METHOD(Aerospike, close) {
	as_error err;
	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	aerospike* as_ptr = NULL;

	as_error_init(&err);
	if (client && client->as_client) {
		as_ptr = client->as_client;
	} else {
		// THIS IS An unrecoverable error
		RETURN_LONG(AEROSPIKE_ERR_CLIENT);
	}
	/* Only actually close the connection if we are not sharing the connection */
	if (!client->is_persistent) {
		aerospike_close(as_ptr, &err);
	}
	client->is_connected = false;

	RETURN_LONG(err.code)
}
/* }}} */


/* add hosts to an aerospike object from the hosts hashtable passed into the constructor
 *
 * Returns AEROSPIKE_OK on success, else an error
 */
static as_status add_hosts_from_zhash(as_config* config, HashTable* z_hosts) {


	HashTable* z_host_entry;
	char* hostname = NULL;
	char* tls_name = NULL;
	zval* z_tls_name = NULL;
	zval* z_hostname = NULL;
	zval* z_port = NULL;
	uint16_t port;

	zval* z_host;

	if (!hashtable_is_list(z_hosts)) {
		zend_throw_exception(NULL, "Hosts entry must be an array", 0);
		return AEROSPIKE_ERR_PARAM;
	}
	/* Add each of the host entries to the as_config.hosts struct */
	ZEND_HASH_FOREACH_VAL(z_hosts,  z_host)
	{
		/* We expect the host entry to be [['hostname', 'port'], ['hostname', 'port']]
		* and not ["hostname1"=>['hostname', 'port']]
		* So verify that it is that way and error out otherwise */

			/* Verify host is of the form ['host', port#, [tls-name]], this will eventually have optional 3rd tls-name */
			if (Z_TYPE_P(z_host) != IS_ARRAY) {
				zend_throw_exception(NULL, "host entry must be an array", 0);
				return AEROSPIKE_ERR_PARAM;
			} else {
				z_host_entry = Z_ARRVAL_P(z_host);
				if (zend_hash_num_elements(z_host_entry) < 2 || zend_hash_num_elements(z_host_entry) > 3) {
					zend_throw_exception(NULL, "host entry must contain 2 or 3 elements", 0);
					return AEROSPIKE_ERR_PARAM;
				}

				hostname = NULL;
				tls_name = NULL;

				z_hostname = zend_hash_str_find(z_host_entry, "addr", strlen("addr"));
				z_port = zend_hash_str_find(z_host_entry, "port", strlen("port"));

				if (!z_hostname || !z_port) {
					zend_throw_exception(NULL, "host entry must contain the keys 'addr' and 'port'", 0);
					return AEROSPIKE_ERR_PARAM;
				}
				/* Handle TLS Name if present */
				if (zend_hash_num_elements(z_host_entry) == 3) {
					z_tls_name = zend_hash_str_find(z_host_entry, "tls_name", strlen("tls_name"));
					if (z_tls_name) {
						if (Z_TYPE_P(z_tls_name) != IS_STRING) {
							zend_throw_exception(NULL, "TLS name must be a string", 0);
							return AEROSPIKE_ERR_PARAM;
						} else {
							tls_name = Z_STRVAL_P(z_tls_name);
						}
					}
				}

				if (Z_TYPE_P(z_hostname) != IS_STRING) {
					zend_throw_exception(NULL, "hostname must be a string", 0);
					return AEROSPIKE_ERR_PARAM;
				}
				hostname = Z_STRVAL_P(z_hostname);

				if (Z_TYPE_P(z_port) != IS_LONG) {
					zend_throw_exception(NULL, "port must be a number", 0);
					return AEROSPIKE_ERR_PARAM;
				}
				port = (uint16_t)Z_LVAL_P(z_port);

				if (tls_name) {
					as_config_tls_add_host(config, hostname, tls_name, port);
				} else {
					as_config_add_host(config, hostname, port);
				}

			}
		/* END HOST ADDITIONS TO CONFIG */
	} ZEND_HASH_FOREACH_END();

	return AEROSPIKE_OK;
}
/*
 * This is a helper function to set the following fields for the constructor
 * username
 * password
 * shm (dict) with following keys:
 * 		shm_key
 * 		shm_max_nodes
 * 		shm_max_namespaces
 * 		shm_takeover_threshold_sec
 * 	max_threads
 * 	thread_pool_size (Always zero when using ZTS)
 * 	compression_threshold
 */
static as_status set_as_config(as_config* config, HashTable* z_conf_hash) {
	HashTable* shm_hash = NULL;
	zval* setting_value = NULL;
	char* user = NULL;
	char* password = NULL;
	/* SHM Settings */

	set_as_config_from_ini(config);
	setting_value = zend_hash_str_find(z_conf_hash, "shm", strlen("shm"));
	if (setting_value) {
		config->use_shm = true;

		if (Z_TYPE_P(setting_value) != IS_ARRAY) {
			return AEROSPIKE_ERR_PARAM;
		}
		shm_hash = Z_ARRVAL_P(setting_value);

		if (shm_hash) {
			setting_value = zend_hash_str_find(shm_hash, "shm_key", strlen("shm_key"));
			if (setting_value) {
				if (Z_TYPE_P(setting_value) != IS_LONG) {
					return AEROSPIKE_ERR_PARAM;
				}
				config->shm_key = Z_LVAL_P(setting_value);
			}

			setting_value = zend_hash_str_find(shm_hash, "shm_max_nodes", strlen("shm_max_nodes"));
			if (setting_value) {
				if (Z_TYPE_P(setting_value) != IS_LONG) {
					return AEROSPIKE_ERR_PARAM;
				}
				config->shm_max_nodes = Z_LVAL_P(setting_value);
			}

			setting_value = zend_hash_str_find(shm_hash, "shm_max_namespaces", strlen("shm_max_namespaces"));
			if (setting_value) {
				if (Z_TYPE_P(setting_value) != IS_LONG) {
					return AEROSPIKE_ERR_PARAM;
				}
				config->shm_max_namespaces = Z_LVAL_P(setting_value);
			}

			setting_value = zend_hash_str_find(shm_hash,"shm_takeover_threshold_sec", strlen("shm_takeover_threshold_sec"));
			if (setting_value) {
				if (Z_TYPE_P(setting_value) != IS_LONG) {
					return AEROSPIKE_ERR_PARAM;
				}
				config->shm_takeover_threshold_sec = Z_LVAL_P(setting_value);
			}
		}
	}

	/* Authentication information */
	setting_value = zend_hash_str_find(z_conf_hash, "user", strlen("user"));
	if (setting_value) {
		if (Z_TYPE_P(setting_value) != IS_STRING) {
			return AEROSPIKE_ERR_PARAM;
		}
		user = Z_STRVAL_P(setting_value);

		setting_value = zend_hash_str_find(z_conf_hash, "pass", strlen("pass"));
		if (setting_value) {
			if (Z_TYPE_P(setting_value) != IS_STRING) {
				return AEROSPIKE_ERR_PARAM;
			}
			password = Z_STRVAL_P(setting_value);

			if (!as_config_set_user(config, user, password)) {
				return AEROSPIKE_ERR_PARAM;
			}
		}
	}

	setting_value = zend_hash_str_find(z_conf_hash, "max_threads", strlen("max_threads"));
	if (setting_value) {
		if (Z_TYPE_P(setting_value) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		config->max_conns_per_node = Z_LVAL_P(setting_value);
	}

	setting_value = zend_hash_str_find(z_conf_hash, "thread_pool_size", strlen("thread_pool_size"));
	if (setting_value) {
		if (Z_TYPE_P(setting_value) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		config->thread_pool_size = Z_LVAL_P(setting_value);
/* Using a larger thead pool will crash zts builds, so force it to be 0 */
#ifdef ZTS
	config->thread_pool_size = 0;
#endif

	}

	setting_value = zend_hash_str_find(z_conf_hash, "compression_threshold", strlen("compression_threshold"));
	if (setting_value) {
		if (Z_TYPE_P(setting_value) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		config->policies.write.compression_threshold = Z_LVAL_P(setting_value);
	}

	return AEROSPIKE_OK;
}

static void set_as_config_from_ini(as_config* config) {

	/*SHM Information*/
	config->use_shm = INI_BOOL("aerospike.shm.use");
	config->shm_key = INI_INT("aerospike.shm.key");
	config->shm_max_namespaces = INI_INT("aerospike.shm.max_namespaces");
	config->shm_takeover_threshold_sec = INI_INT("aerospike.shm.takeover_threshold_sec");
	config->shm_max_nodes = INI_INT("aerospike.shm.max_nodes");

#ifdef ZTS
	config->thread_pool_size = 0;
#else
	config->thread_pool_size = INI_INT("aerospike.thread_pool_size");
#endif
	config->max_conns_per_node = INI_INT("aerospike.max_threads");

	config->policies.write.compression_threshold = INI_INT("aerospike.compression_threshold");

	return;

}

/*
 * Helper function which sets as_config entries from a hashtable passed into the Aerospike constructor
 */
static as_status set_policy_defaults_from_hash(as_config* config, AerospikeClient* client, HashTable* policy_hash) {
	zval* policy_zval = NULL;
	long int_ini_value;

	if (!policy_hash) {
		return AEROSPIKE_OK;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_CONNECT_TIMEOUT);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		config->conn_timeout_ms = Z_LVAL_P(policy_zval);
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_READ_TIMEOUT);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.read.timeout = int_ini_value;
		config->policies.info.timeout = int_ini_value;
		config->policies.batch.timeout = int_ini_value;
		config->policies.scan.timeout = int_ini_value;
		config->policies.query.timeout = int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_WRITE_TIMEOUT);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.write.timeout = int_ini_value;
		config->policies.operate.timeout = int_ini_value;
		config->policies.remove.timeout = int_ini_value;
		config->policies.apply.timeout = int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_POLICY_KEY);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.key = (as_policy_key)int_ini_value;
		config->policies.read.key = (as_policy_key)int_ini_value;
		config->policies.write.key = (as_policy_key)int_ini_value;
		config->policies.operate.key = (as_policy_key)int_ini_value;
		config->policies.remove.key = (as_policy_key)int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_POLICY_EXISTS);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.exists = (as_policy_exists)int_ini_value;
		config->policies.write.exists = (as_policy_exists)int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_POLICY_RETRY);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.retry = (as_policy_retry)int_ini_value;
		config->policies.write.retry = (as_policy_retry)int_ini_value;
		config->policies.operate.retry = (as_policy_retry)int_ini_value;
		config->policies.remove.retry = (as_policy_retry)int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_POLICY_COMMIT_LEVEL);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.commit_level = (as_policy_commit_level)int_ini_value;
		config->policies.write.commit_level = (as_policy_commit_level)int_ini_value;
		config->policies.remove.commit_level = (as_policy_commit_level)int_ini_value;
		config->policies.operate.commit_level = (as_policy_commit_level)int_ini_value;
		config->policies.apply.commit_level = (as_policy_commit_level)int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_POLICY_CONSISTENCY);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.consistency_level = (as_policy_consistency_level)int_ini_value;
		config->policies.read.consistency_level = (as_policy_consistency_level)int_ini_value;
		config->policies.operate.consistency_level = (as_policy_consistency_level)int_ini_value;
		config->policies.batch.consistency_level = (as_policy_consistency_level)int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_POLICY_REPLICA);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		int_ini_value = Z_LVAL_P(policy_zval);
		config->policies.replica = (as_policy_replica)int_ini_value;
		config->policies.read.replica = (as_policy_replica)int_ini_value;
		config->policies.operate.replica = (as_policy_replica)int_ini_value;
	}

	policy_zval = zend_hash_index_find(policy_hash, OPT_SERIALIZER);
	if (policy_zval) {
		if (Z_TYPE_P(policy_zval) != IS_LONG) {
			return AEROSPIKE_ERR_PARAM;
		}
		client->serializer_type = Z_LVAL_P(policy_zval);
	}
	return AEROSPIKE_OK;
}
