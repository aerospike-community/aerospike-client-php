// *****************************************************************************
// Copyright 2018 Aerospike, Inc.
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

#include "tls_config.h"
#include "php_aerospike_types.h"

/*
 * Set the values of as_config_tls fields from a PHP HashTable
 */
as_status setTLSConfig(as_config* config, const HashTable* tlsHash) {

	zval* z_conf_val = NULL;

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_ENABLE);
	if (z_conf_val) {
		if ( (Z_TYPE_P(z_conf_val) == IS_TRUE) || (Z_TYPE_P(z_conf_val) == IS_FALSE) ) {
			config->tls.enable = Z_TYPE_P(z_conf_val) == IS_TRUE;
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CAFILE);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_cafile(config, Z_STRVAL_P(z_conf_val));
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CAPATH);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_capath(config, Z_STRVAL_P(z_conf_val));
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_PROTOCOLS);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_protocols(config, Z_STRVAL_P(z_conf_val));
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CIPHER_SUITE);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_cipher_suite(config, Z_STRVAL_P(z_conf_val));
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CRL_CHECK);
	if (z_conf_val) {
		if ( (Z_TYPE_P(z_conf_val) == IS_TRUE) || (Z_TYPE_P(z_conf_val) == IS_FALSE) ) {
			config->tls.crl_check = Z_TYPE_P(z_conf_val) == IS_TRUE;
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CRL_CHECK_ALL);
	if (z_conf_val) {
		if ( (Z_TYPE_P(z_conf_val) == IS_TRUE) || (Z_TYPE_P(z_conf_val) == IS_FALSE) ) {
			config->tls.crl_check_all = Z_TYPE_P(z_conf_val) == IS_TRUE;
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CERT_BLACKLIST);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_cert_blacklist(config, Z_STRVAL_P(z_conf_val));
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_LOG_SESSION_INFO);
	if (z_conf_val) {
		if ( (Z_TYPE_P(z_conf_val) == IS_TRUE) || (Z_TYPE_P(z_conf_val) == IS_FALSE) ) {
			config->tls.log_session_info = Z_TYPE_P(z_conf_val) == IS_TRUE;
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_KEYFILE);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_keyfile(config, Z_STRVAL_P(z_conf_val));
		}
	}

	z_conf_val = zend_hash_index_find(tlsHash, OPT_TLS_CERTFILE);
	if (z_conf_val) {
		if (Z_TYPE_P(z_conf_val) == IS_STRING) {
			as_config_tls_set_certfile(config, Z_STRVAL_P(z_conf_val));
		}
	}



	return AEROSPIKE_OK;
}

