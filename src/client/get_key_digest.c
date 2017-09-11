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
#include "conversions.h"
#include "php_aerospike_types.h"


/* {{{ proto string Aerospike::getKeyDigest( string ns, string set, int|string pk )
    Helper which computes the digest that for a given key */
PHP_METHOD(Aerospike, getKeyDigest)
{
	char* ns_str = NULL;
	char* set_str = NULL;
	zval* primary_key = NULL;
	bool key_initialized = false;
	size_t ns_len;
	size_t set_len;
	as_error err;
	zval z_key_array;

	as_key key;
	as_digest *digest = NULL;
	size_t len;
	reset_client_error(getThis());
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssz",
							  &ns_str, &ns_len, &set_str,
							  &set_len, &primary_key) != SUCCESS) {
		return;
	}

	if ((Z_TYPE_P(primary_key) != IS_STRING) && (Z_TYPE_P(primary_key) != IS_LONG)) {
		return; // Invalid key here
	}
	
	as_error_init(&err);
	array_init(&z_key_array);

	add_assoc_string(&z_key_array, NAMESPACE_KEY, ns_str);
	add_assoc_string(&z_key_array, SET_KEY, set_str);
	// Because we are deleting the array and it may be a string,
	// We need to add a reference in order to prevent a double free
	Z_TRY_ADDREF_P(primary_key);
	add_assoc_zval(&z_key_array, PK_KEY, primary_key);


	if (z_hashtable_to_as_key(Z_ARRVAL_P(&z_key_array), &key, &err) == AEROSPIKE_OK) {
		key_initialized = true;
	} else {
		goto CLEANUP;
	}

	digest = as_key_digest(&key);
	if (digest && digest->init) {
		len = sizeof(digest->value);
		ZVAL_STRINGL(return_value, (char*)(digest->value), len);
	}

CLEANUP:
	if (key_initialized) {
		as_key_destroy(&key);
	}
	zval_dtor(&z_key_array);
}
/* }}} */
