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

#include "php.h"
#include "aerospike/as_status.h"
#include "aerospike/as_bytes.h"
#include "aerospike/as_error.h"

as_status serialize_with_user_function(const zval* zval_to_serialize, as_bytes** serialized_bytes, as_error* err, int serializer_type);
as_status unserialize_with_user_function(const as_bytes* bytes, zval* return_zval, as_error* err);
