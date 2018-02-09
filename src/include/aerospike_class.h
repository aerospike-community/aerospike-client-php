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

/*
This includes the functions and arguments needed for all of the aerospike class methods
*/

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_aerospike.h"

#define AS_PHP_UPDATE_CLIENT_ERROR(client, code, message) as_error_set_message(&((client)->client_error), code, message)
#define AS_PHP_RESET_CLIENT_ERROR(client) as_error_set_message(&((client)->client_error), AEROSPIKE_OK, "")
extern zend_class_entry *aerospike_ce;
extern zend_object_handlers aerospike_ce_handlers;

bool register_aerospike_class(void);
zend_object *aerospike_create_object(zend_class_entry *ce);


PHP_METHOD(Aerospike, __construct);
ZEND_BEGIN_ARG_INFO_EX(construct_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, addIndex);
ZEND_BEGIN_ARG_INFO_EX(add_index_arg_info, 0, 0, 6)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, append);
ZEND_BEGIN_ARG_INFO_EX(append_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, apply);
ZEND_BEGIN_ARG_INFO_EX(apply_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, close);
ZEND_BEGIN_ARG_INFO_EX(close_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, deregister);
ZEND_BEGIN_ARG_INFO_EX(deregister_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, dropIndex);
ZEND_BEGIN_ARG_INFO_EX(drop_index_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, error);
ZEND_BEGIN_ARG_INFO_EX(error_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, errorno);
ZEND_BEGIN_ARG_INFO_EX(errorno_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, errorInDoubt);
ZEND_BEGIN_ARG_INFO_EX(error_in_doubt_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, exists);
ZEND_BEGIN_ARG_INFO_EX(exists_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, get);
ZEND_BEGIN_ARG_INFO_EX(get_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, getKeyDigest);
ZEND_BEGIN_ARG_INFO_EX(getKeyDigest_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, isConnected);
ZEND_BEGIN_ARG_INFO_EX(is_connected_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, increment);
ZEND_BEGIN_ARG_INFO_EX(increment_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, initKey);
ZEND_BEGIN_ARG_INFO_EX(initkey_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, jobInfo);
ZEND_BEGIN_ARG_INFO_EX(job_info_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listAppend);
ZEND_BEGIN_ARG_INFO_EX(list_append_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listClear);
ZEND_BEGIN_ARG_INFO_EX(list_clear_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listGet);
ZEND_BEGIN_ARG_INFO_EX(list_get_arg_info, 0, 0, 4)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listGetRange);
ZEND_BEGIN_ARG_INFO_EX(list_get_range_arg_info, 0, 0, 5)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listInsert);
ZEND_BEGIN_ARG_INFO_EX(list_insert_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listInsertItems);
ZEND_BEGIN_ARG_INFO_EX(list_insert_items_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listMerge);
ZEND_BEGIN_ARG_INFO_EX(list_merge_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listPop);
ZEND_BEGIN_ARG_INFO_EX(list_pop_arg_info, 0, 0, 4)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listPopRange);
ZEND_BEGIN_ARG_INFO_EX(list_pop_range_arg_info, 0, 0, 5)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listRemove);
ZEND_BEGIN_ARG_INFO_EX(list_remove_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listRemoveRange);
ZEND_BEGIN_ARG_INFO_EX(list_remove_range_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listSet);
ZEND_BEGIN_ARG_INFO_EX(list_set_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listSize);
ZEND_BEGIN_ARG_INFO_EX(list_size_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listTrim);
ZEND_BEGIN_ARG_INFO_EX(list_trim_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, listRegistered);
ZEND_BEGIN_ARG_INFO_EX(list_registered_arg_info, 0, 0, 1)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, operate);
ZEND_BEGIN_ARG_INFO_EX(operate_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, operateOrdered);
ZEND_BEGIN_ARG_INFO_EX(operate_ordered_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, put);
ZEND_BEGIN_ARG_INFO_EX(put_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, prepend);
ZEND_BEGIN_ARG_INFO_EX(prepend_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, reconnect);
ZEND_BEGIN_ARG_INFO_EX(reconnect_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, register);
ZEND_BEGIN_ARG_INFO_EX(register_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, remove);
ZEND_BEGIN_ARG_INFO_EX(remove_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, removeBin);
ZEND_BEGIN_ARG_INFO_EX(remove_bin_arg_info, 0, 0, 2)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, bins)
ZEND_ARG_INFO(0, write_policy)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, shmKey);
ZEND_BEGIN_ARG_INFO_EX(shm_key_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, select);
ZEND_BEGIN_ARG_INFO_EX(select_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, setDeserializer);
ZEND_BEGIN_ARG_INFO_EX(set_deserializer_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, setSerializer);
ZEND_BEGIN_ARG_INFO_EX(set_serializer_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, touch);
ZEND_BEGIN_ARG_INFO_EX(touch_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, truncate);
ZEND_BEGIN_ARG_INFO_EX(truncate_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

/* UDF METHODS */
PHP_METHOD(Aerospike, getRegistered);
ZEND_BEGIN_ARG_INFO_EX(get_registered_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

/*Predicate Methods*/
PHP_METHOD(Aerospike, predicateEquals);
ZEND_BEGIN_ARG_INFO_EX(predicate_equals_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateGeoContainsGeoJSONPoint);
ZEND_BEGIN_ARG_INFO_EX(predicate_geo_contains_json_point_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateGeoContainsPoint);
ZEND_BEGIN_ARG_INFO_EX(predicate_geo_contains_point_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateGeoWithinGeoJSONRegion);
ZEND_BEGIN_ARG_INFO_EX(predicate_geo_within_region_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateGeoWithinRadius);
ZEND_BEGIN_ARG_INFO_EX(predicate_geo_within_radius_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateRange);
ZEND_BEGIN_ARG_INFO_EX(predicate_range_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateBetween);
ZEND_BEGIN_ARG_INFO_EX(predicate_between_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, predicateContains);
ZEND_BEGIN_ARG_INFO_EX(predicate_contains_arg_info, 0, 0, 2)
ZEND_END_ARG_INFO();

/*Logging methods */
PHP_METHOD(Aerospike, setLogLevel);
ZEND_BEGIN_ARG_INFO_EX(set_log_level_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, setLogHandler);
ZEND_BEGIN_ARG_INFO_EX(set_log_handler_arg_info, 0, 0, 1)
ZEND_END_ARG_INFO();

/*Info Methods*/

PHP_METHOD(Aerospike, getNodes);
ZEND_BEGIN_ARG_INFO_EX(get_nodes_arg_info, 0, 0, 0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, info);
ZEND_BEGIN_ARG_INFO_EX(info_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, infoMany);
ZEND_BEGIN_ARG_INFO_EX(info_many_arg_info, 0, 0, 1)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

/*SCAN/QUERY METHODS*/
PHP_METHOD(Aerospike, scan);
ZEND_BEGIN_ARG_INFO_EX(scan_arg_info, 0, 0, 3)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, scanApply);
ZEND_BEGIN_ARG_INFO_EX(scan_apply_arg_info, 0, 0, 6)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, scanInfo);
ZEND_BEGIN_ARG_INFO_EX(scan_info_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, query);
ZEND_BEGIN_ARG_INFO_EX(query_arg_info, 0, 0, 4)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, queryApply);
ZEND_BEGIN_ARG_INFO_EX(query_apply_arg_info, 0, 0, 7)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, aggregate);
ZEND_BEGIN_ARG_INFO_EX(aggregate_arg_info, 0, 0, 7)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO();

/*BATCH METHODS*/
PHP_METHOD(Aerospike, existsMany);
ZEND_BEGIN_ARG_INFO_EX(exists_many_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, getMany);
ZEND_BEGIN_ARG_INFO_EX(get_many_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

/*Security Methods*/

PHP_METHOD(Aerospike, changePassword);
ZEND_BEGIN_ARG_INFO_EX(change_password_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, setPassword);
ZEND_BEGIN_ARG_INFO_EX(set_password_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, createRole);
ZEND_BEGIN_ARG_INFO_EX(create_role_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, createUser);
ZEND_BEGIN_ARG_INFO_EX(create_user_arg_info, 0, 0, 3)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, queryUser);
ZEND_BEGIN_ARG_INFO_EX(query_user_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, queryUsers);
ZEND_BEGIN_ARG_INFO_EX(query_users_arg_info, 0, 0, 1)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();


PHP_METHOD(Aerospike, queryRole);
ZEND_BEGIN_ARG_INFO_EX(query_role_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, queryRoles);
ZEND_BEGIN_ARG_INFO_EX(query_roles_arg_info, 0, 0, 1)
ZEND_ARG_PASS_INFO(1)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, dropRole);
ZEND_BEGIN_ARG_INFO_EX(drop_role_arg_info, 0, 0, 1)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, grantPrivileges);
ZEND_BEGIN_ARG_INFO_EX(grant_privileges_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, revokePrivileges);
ZEND_BEGIN_ARG_INFO_EX(revoke_privileges_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, grantRoles);
ZEND_BEGIN_ARG_INFO_EX(grant_roles_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, revokeRoles);
ZEND_BEGIN_ARG_INFO_EX(revoke_roles_arg_info, 0, 0, 2)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

PHP_METHOD(Aerospike, dropUser);
ZEND_BEGIN_ARG_INFO_EX(drop_user_arg_info, 0, 0, 1)
ZEND_ARG_PASS_INFO(0)
ZEND_ARG_PASS_INFO(0)
ZEND_END_ARG_INFO();

