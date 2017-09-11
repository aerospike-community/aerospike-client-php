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
#ifndef AS_PHP_PERSISTENT_LIST_H
#define AS_PHP_PERSISTENT_LIST_H
#include "php.h"
#include "php_aerospike_types.h"

typedef struct _persistent_host {
	aerospike* as_client;
	int ref_cnt;
	int conn_cnt;
	bool is_connected;
} persistent_host;

char* get_persistent_search_string(aerospike* as);
persistent_host* add_persistent_host(const char* host_key, aerospike* as_client);
persistent_host* get_persistent_host(const char* host_key);
void persistent_host_dtor(zval* zval);
as_status persistent_connect(AerospikeClient* client);
as_status non_persistent_connect(AerospikeClient* client);
#endif
