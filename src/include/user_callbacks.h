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
#include "php.h"
#include "aerospike/as_record.h"
#include "pthread.h"

typedef struct _user_callback_function {
	as_error* err;
	zend_fcall_info callback;
	zend_fcall_info_cache callback_cache;
	pthread_mutex_t* cb_mutex;
} user_callback_function;

as_status execute_user_callback(as_record* record, user_callback_function* callback);
bool user_callback_wrapper(const as_val *val, void *udata);
