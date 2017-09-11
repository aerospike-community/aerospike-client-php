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
void update_client_error(zval* client_obj, int code, const char* msg);
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

enum Aerospike_opt_keys {
	OPT_CONNECT_TIMEOUT = 1, /* value in milliseconds, default: 1000                                          */
	OPT_READ_TIMEOUT,        /* value in milliseconds, default: 1000                                          */
	OPT_WRITE_TIMEOUT,       /* value in milliseconds, default: 1000                                          */
	OPT_POLICY_RETRY,        /* set to a Aerospike::POLICY_RETRY_* value                                      */
	OPT_POLICY_EXISTS,       /* set to a Aerospike::POLICY_EXISTS_* value                                     */
	OPT_SERIALIZER,          /* set the unsupported type handler                                              */
	OPT_SCAN_PRIORITY,       /* set to a Aerospike::SCAN_PRIORITY_* value                                     */
	OPT_SCAN_PERCENTAGE,     /* integer value 1-100, default: 100                                             */
	OPT_SCAN_CONCURRENTLY,   /* boolean value, default: false                                                 */
	OPT_SCAN_NOBINS,         /* boolean value, default: false                                                 */
	OPT_SCAN_INCLUDELDT,     /* Include large data type bin values in addition to large data type bin names   */
	OPT_POLICY_KEY,          /* records store the digest unique ID, optionally also its (ns, set, key) inputs */
	OPT_POLICY_GEN,          /* set to one of Aerospike::OPT_POLICY_GEN_*                                     */
	OPT_POLICY_REPLICA,      /* set to one of Aerospike::POLICY_REPLICA_*                                     */
	OPT_POLICY_CONSISTENCY,  /* set to one of Aerospike::POLICY_CONSISTENCY_*                                 */
	OPT_POLICY_COMMIT_LEVEL, /* set to one of Aerospike::POLICY_COMMIT_LEVEL_*                                */
	OPT_TTL,                 /* set to time-to-live of the record in seconds                                  */
	USE_BATCH_DIRECT,        /* use new batch index protocol if server supports it                            */
	COMPRESSION_THRESHOLD,   /* Minimum record size beyond which it is compressed and sent to the server      */
	OPT_POLICY_DURABLE_DELETE /* set to true to enable durable delete for the operation                       */
};

#endif
