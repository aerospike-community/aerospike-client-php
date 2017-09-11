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

#include "aerospike_class.h"
#include "php_aerospike_types.h"

PHP_METHOD(Aerospike, setSerializer)
{
	zend_fcall_info serializer_info;
	zend_fcall_info_cache serializer_cache;


	if(zend_parse_parameters(ZEND_NUM_ARGS(), "f", &serializer_info, &serializer_cache) != SUCCESS){
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	// If an old serializer existed, we need to get rid of it gracefully
	// If there is a serializer set, unset it here, otherwise this a
	if AEROSPIKE_G(is_global_user_serializer_registered) {
		zval_dtor(&AEROSPIKE_G(user_global_serializer_call_info).function_name);
		AEROSPIKE_G(is_global_user_serializer_registered) = false;
	}
	memcpy(&AEROSPIKE_G(user_global_serializer_call_info), &serializer_info, sizeof(zend_fcall_info));
	AEROSPIKE_G(is_global_user_serializer_registered) = true;

	Z_TRY_ADDREF(serializer_info.function_name);

	memcpy(&AEROSPIKE_G(user_global_serializer_call_info_cache), &serializer_cache, sizeof(zend_fcall_info_cache));

	RETURN_LONG(AEROSPIKE_OK);
}

PHP_METHOD(Aerospike, setDeserializer)
{
	zend_fcall_info deserializer_info;
	zend_fcall_info_cache deserializer_cache;


	if(zend_parse_parameters(ZEND_NUM_ARGS(), "f", &deserializer_info, &deserializer_cache) != SUCCESS) {
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	// If an old deserializer existed, we need to get rid of it gracefully
	if(AEROSPIKE_G(is_global_user_deserializer_registered)) {
		zval_dtor(&AEROSPIKE_G(user_global_deserializer_call_info).function_name);
	}

	memcpy(&AEROSPIKE_G(user_global_deserializer_call_info), &deserializer_info, sizeof(zend_fcall_info));
	Z_TRY_ADDREF(deserializer_info.function_name);

	memcpy(&AEROSPIKE_G(user_global_deserializer_call_info_cache), &deserializer_cache, sizeof(zend_fcall_info_cache));
	AEROSPIKE_G(is_global_user_deserializer_registered) = true;

	RETURN_LONG(AEROSPIKE_OK);
}
