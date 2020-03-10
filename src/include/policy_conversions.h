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

/* policy_conversions.h */
#include "php.h"
#include "c_aerospike_types.h"
#include "aerospike/as_scan.h"
#include "aerospike/as_query.h"
#include "aerospike/as_map_operations.h"

as_status zval_to_as_policy_apply(zval* z_info_policy, as_policy_apply* apply_policy,
								  as_policy_apply** apply_policy_p, as_policy_apply* default_policy);

as_status zval_to_as_policy_info(zval* z_info_policy, as_policy_info* info_policy,
								 as_policy_info** info_policy_p, as_policy_info* default_policy);

as_status zval_to_as_policy_operate(zval* z_policy, as_policy_operate* operate_policy,
									as_policy_operate** operate_policy_p, as_policy_operate* default_policy);

as_status zval_to_as_policy_read(zval* z_policy, as_policy_read* read_policy,
								 as_policy_read** read_policy_p, as_policy_read* default_policy);

as_status zval_to_as_policy_remove(zval* z_policy, as_policy_remove* remove_policy,
								   as_policy_remove** remove_policy_p, as_policy_remove* default_policy);

as_status zval_to_as_policy_scan(zval* z_policy, as_policy_scan* scan_policy,
								 as_policy_scan** scan_policy_p, as_policy_scan* default_policy);

as_status zval_to_as_policy_query(zval* z_policy, as_policy_query* query_policy,
								  as_policy_query** query_policy_p, as_policy_query* default_policy);

as_status zval_to_as_policy_write(zval* z_policy, as_policy_write* write_policy,
								  as_policy_write** write_policy_p, as_policy_write* default_policy);

as_status zval_to_as_policy_batch(zval* z_policy, as_policy_batch* batch_policy,
								  as_policy_batch** batch_policy_p, as_policy_batch* default_policy);

as_status zval_to_as_policy_admin(zval* z_policy, as_policy_admin* admin_policy,
								  as_policy_admin** admin_policy_p, as_policy_admin* default_policy);

as_status
zval_to_as_policy_map(zval* z_policy, as_map_policy* map_policy);

// The following functions initialize a policy object with INI entries
as_status set_serializer_from_policy_hash(int* serializer_type, zval* z_policy);
as_status set_deserializer_from_policy_hash(int* deserializer_type, zval* z_policy);
as_status set_record_generation_from_write_policy(as_record* record, zval* z_write_policy);
as_status set_operations_generation_from_operate_policy(as_operations* operations, zval* z_write_policy);
as_status set_operations_ttl_from_operate_policy(as_operations* operations, zval* z_write_policy);
as_status set_scan_options_from_policy_hash(as_scan* scan, zval* z_policy);
as_status set_query_options_from_policy_hash(as_query* query, zval* z_policy);

/* Functions used in the class constructor */
as_status set_read_policy_from_hash(HashTable* z_policy_hash, as_policy_read* read_policy);
as_status set_write_policy_from_hash(HashTable* z_policy_hash, as_policy_write* write_policy);
as_status set_remove_policy_from_hash(HashTable* z_policy_hash, as_policy_remove* remove_policy);
as_status set_batch_policy_from_hash(HashTable* z_policy_hash, as_policy_batch* batch_policy);
as_status set_operate_policy_from_hash(HashTable* z_policy_hash, as_policy_operate* operate_policy);
as_status set_query_policy_from_hash(HashTable* z_policy_hash, as_policy_query* query_policy);
as_status set_scan_policy_from_hash(HashTable* z_policy_hash, as_policy_scan* scan_policy);
as_status set_apply_policy_from_hash(HashTable* z_policy_hash, as_policy_apply* apply_policy);
