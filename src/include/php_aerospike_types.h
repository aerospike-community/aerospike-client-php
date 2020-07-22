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

/**types.h*/
#pragma once
#ifndef AEROSPIKE_PHP_AEROSPIKE_TYPES_H
#define AEROSPIKE_PHP_AEROSPIKE_TYPES_H
#include "aerospike/aerospike.h"
#include "aerospike/as_error.h"

#include "php.h"

// Keys in the php hash form of a key
#define NAMESPACE_KEY "ns"
#define SET_KEY "set"
#define PK_KEY "key"
#define DIGEST_KEY "digest"

// values used to create query where clauses
#define PHP_PREDICATE_EQUAL "="
#define PHP_PREDICATE_BETWEEN "BETWEEN"
#define PHP_PREDICATE_CONTAINS "CONTAINS"
#define PHP_PREDICATE_RANGE "RANGE"
#define PHP_PREDICATE_GEO_WITHIN "GEOWITHIN"
#define PHP_PREDICATE_GEO_CONTAINS "GEOCONTAINS"

// String policy Constants
#define PHP_POLICY_OPT_DESERIALIZE "deserialize"
#define PHP_POLICY_OPT_SLEEP_BETWEEN_RETRIES "sleep_between_retries"

#define SERIALIZER_DEFAULT "php"

typedef struct aerospike_client_z {
	aerospike* as_client;
	bool is_connected;
	bool is_valid;
	as_error client_error;
	bool is_persistent;
	int serializer_type;
	zend_object zobj;
}AerospikeClient;

enum Aerospike_serializer_values {
	SERIALIZER_NONE,
	SERIALIZER_PHP, /* default handler for serializer type */
	SERIALIZER_USER,
};

AerospikeClient* get_aerospike_from_zobj(zend_object* zval_wrapper);
void update_client_error(zval* client_obj, int code, const char* msg, bool in_doubt);
void reset_client_error(zval* client_obj);
as_status check_object_and_connection(zval* aerospike_container, as_error* err);
void set_policy_defaults_from_ini(as_config* config, AerospikeClient* client);

enum Aerospike_list_operations {
	OP_LIST_APPEND = 1001,
	OP_LIST_MERGE,
	OP_LIST_INSERT,
	OP_LIST_INSERT_ITEMS,
	OP_LIST_POP,
	OP_LIST_POP_RANGE,
	OP_LIST_REMOVE,
	OP_LIST_REMOVE_RANGE,
	OP_LIST_CLEAR,
	OP_LIST_SET,
	OP_LIST_GET,
	OP_LIST_GET_RANGE,
	OP_LIST_TRIM,
	OP_LIST_SIZE
};

enum Aerospike_map_operations {
	OP_MAP_SET_POLICY = 1101,
	OP_MAP_PUT,
	OP_MAP_PUT_ITEMS,
	OP_MAP_INCREMENT,
	OP_MAP_DECREMENT,
	OP_MAP_SIZE,
	OP_MAP_CLEAR,
	OP_MAP_REMOVE_BY_KEY,
	OP_MAP_REMOVE_BY_KEY_LIST,
	OP_MAP_REMOVE_BY_KEY_RANGE,
	OP_MAP_REMOVE_BY_VALUE,
	OP_MAP_REMOVE_BY_VALUE_LIST,
	OP_MAP_REMOVE_BY_VALUE_RANGE,
	OP_MAP_REMOVE_BY_INDEX,
	OP_MAP_REMOVE_BY_INDEX_RANGE,
	OP_MAP_REMOVE_BY_RANK,
	OP_MAP_REMOVE_BY_RANK_RANGE,
	OP_MAP_GET_BY_KEY,
	OP_MAP_GET_BY_KEY_RANGE,
	OP_MAP_GET_BY_VALUE,
	OP_MAP_GET_BY_VALUE_RANGE,
	OP_MAP_GET_BY_INDEX,
	OP_MAP_GET_BY_INDEX_RANGE,
	OP_MAP_GET_BY_RANK,
	OP_MAP_GET_BY_RANK_RANGE,
};

enum Aerospike_opt_keys {
	OPT_CONNECT_TIMEOUT = 1, /* value in milliseconds, default: 1000                                          */
	OPT_READ_TIMEOUT,        /* value in milliseconds, default: 1000                                          */
	OPT_WRITE_TIMEOUT,       /* value in milliseconds, default: 1000                                          */
	OPT_POLICY_RETRY,        /* set to a Aerospike::POLICY_RETRY_* value                                      */
	OPT_POLICY_EXISTS,       /* set to a Aerospike::POLICY_EXISTS_* value                                     */
	OPT_SERIALIZER,          /* set the unsupported type handler                                              */
	OPT_SCAN_PRIORITY,       /* set to a Aerospike::SCAN_PRIORITY_* value                                     */
	OPT_SCAN_RPS_LIMIT,      /* set the results per second(RPS) limit */
	OPT_SCAN_PERCENTAGE,     /* integer value 1-100, default: 100                                             */
	OPT_SCAN_CONCURRENTLY,   /* boolean value, default: false                                                 */
	OPT_SCAN_NOBINS,         /* boolean value, default: false                                                 */
	OPT_SCAN_INCLUDELDT,     /* Include large data type bin values in addition to large data type bin names   */
	OPT_POLICY_KEY,          /* records store the digest unique ID, optionally also its (ns, set, key) inputs */
	OPT_POLICY_GEN,          /* set to one of Aerospike::OPT_POLICY_GEN_*                                     */
	OPT_POLICY_REPLICA,      /* set to one of Aerospike::POLICY_REPLICA_*                                     */
	OPT_POLICY_READ_MODE_AP,  /* set to one of Aerospike::POLICY_READ_MODE_AP_*                                 */
	OPT_POLICY_READ_MODE_SC,  /* set to one of Aerospike::POLICY_READ_MODE_SC_*                                 */
	OPT_POLICY_COMMIT_LEVEL, /* set to one of Aerospike::POLICY_COMMIT_LEVEL_*                                */
	OPT_TTL,                 /* set to time-to-live of the record in seconds                                  */
	USE_BATCH_DIRECT,        /* use new batch index protocol if server supports it                            */
      USE_SERVICES_ALTERNATE,  /* boolean value, default: false                                                 */
	COMPRESSION_THRESHOLD,   /* Minimum record size beyond which it is compressed and sent to the server      */
	OPT_POLICY_DURABLE_DELETE, /* set to true to enable durable delete for the operation                       */
	OPT_SOCKET_TIMEOUT,       /* Socket timeout to be applied to server as well as client */
	OPT_MAP_ORDER,            /* Ordering for an as_map */
	OPT_MAP_WRITE_MODE,        /* Write mode for as_maps */
	OPT_MAP_WRITE_FLAGS,     /* Write flags for as_maps */
	OPT_TOTAL_TIMEOUT,
	OPT_MAX_RETRIES,
	OPT_FAIL_ON_CLUSTER_CHANGE,
	OPT_BATCH_CONCURRENT,
	OPT_ALLOW_INLINE,
	OPT_SEND_SET_NAME,
	OPT_TLS_CONFIG,
	OPT_TLS_ENABLE,
	OPT_TLS_CAFILE,
	OPT_TLS_CAPATH,
	OPT_TLS_PROTOCOLS,
	OPT_TLS_CIPHER_SUITE,
	OPT_TLS_CRL_CHECK,
	OPT_TLS_CRL_CHECK_ALL,
	OPT_TLS_CERT_BLACKLIST,
	OPT_TLS_LOG_SESSION_INFO,
	OPT_TLS_KEYFILE,
	OPT_TLS_CERTFILE,
	OPT_READ_DEFAULT_POL,
	OPT_WRITE_DEFAULT_POL,
	OPT_REMOVE_DEFAULT_POL,
	OPT_BATCH_DEFAULT_POL,
	OPT_OPERATE_DEFAULT_POL,
	OPT_QUERY_DEFAULT_POL,
	OPT_SCAN_DEFAULT_POL,
	OPT_APPLY_DEFAULT_POL,
	OPT_QUERY_NOBINS
};

#endif
