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

#pragma once
#ifndef AEROSPIKE_PHP_CONVERSIONS_H
#define AEROSPIKE_PHP_CONVERSIONS_H
#include "php.h"
#include "ext/standard/info.h"
#include "php_aerospike.h"
#include "c_aerospike_types.h"
#include <stdbool.h>

/* All of these need an error entry so it can be checked on return
*
*/


as_status as_val_to_zval(const as_val* aerospike_value, zval* return_val, as_error* err);
as_status as_key_to_zval(const as_key* key, zval* z_key, bool show_pk, as_error* err);
as_status as_list_to_zval(as_list* list, zval* z_list, as_error* err);
as_status as_map_to_zval(as_map* map, zval* z_map, as_error* err);
as_status as_record_to_zval(const as_record* record, zval* z_record, const as_key* record_key, bool show_pk, as_error* err);
as_status as_record_to_z_metadata(const as_record* record, zval* z_meta, as_error* err);
as_status as_operate_record_to_zval(const as_record* record, zval* z_bins, as_error* err);
as_status as_bins_to_zval(const as_record* aerospike_record, zval* z_bins, as_error* err);

as_status zval_to_as_val(zval* zval_to_convert, as_val** retval, as_error* err, int serializer_type);

as_status z_hashtable_to_as_key(HashTable* z_key_hash, as_key* key, as_error* err);
as_status z_hashtable_to_as_list(HashTable* php_hash, as_list** list, as_error* err, int serializer_type);
as_status z_hashtable_to_as_map(HashTable* php_hash, as_map** c_map, as_error* err, int serializer_type);
as_status z_hashtable_to_as_record(HashTable* z_record_hash, as_record** record, as_error* err, int serializer_type);
as_status add_zval_to_record(zval* add_zval, as_record* record, const char* bin, as_error* err, int serializer_type);

as_status z_hash_to_str_array(HashTable* z_roles, char** roles, int max_size, int roles_size, as_error* err);
as_status z_hashtable_to_as_privileges(HashTable* z_priv_hash, as_privilege** privileges, as_error* err);
as_status as_role_to_zval(as_role* role, zval* z_role, as_error* err);
as_status as_roles_to_zval(as_role** roles, int roles_size, zval* z_roles, as_error* err);
as_status as_privilege_to_zval(as_privilege* privilege, zval* z_privilege, as_error* err);

//utility to see if we are dealing with an iterable with string keys, or a list
bool hashtable_is_list(HashTable* map_or_list);

as_status as_geojson_to_zval(as_geojson* geojson, zval* retval, as_error* err);
as_status as_bytes_to_zval_bytes(const as_bytes* bytes, zval* retval, as_error* err);
as_status serialize_php_object(zval* zval_to_convert, as_bytes** out_bytes, as_error* err, int serializer_type);
as_status unserialize_as_bytes(const as_bytes* bytes, zval* retval, as_error* err);

#endif
