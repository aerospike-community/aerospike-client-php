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
#include "policy_conversions.h"

static as_status as_get_with_bins(aerospike* as, as_error* err, const as_key* key, as_policy_read* read_policy,
		HashTable* bins, as_record** record);

/* {{{ proto int Aerospike::get( array key, array record [, array filter [,array options]] )
    Reads a record from the cluster */
PHP_METHOD(Aerospike, get)
{
	as_key key;
	as_error err;

	zval* get_record = NULL;
	zval* z_read_policy = NULL;
	zval* z_filter = NULL;
	HashTable* z_filter_bins = NULL;
	HashTable* z_key_hash = NULL;

	as_policy_read read_policy;
	as_policy_read* read_policy_p = NULL;

	as_record* record = NULL;

	bool key_initialized = false;

	reset_client_error(getThis());
	as_error_init(&err);

	AerospikeClient* client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
	}
	aerospike* as_ptr = client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "hz/|z!z",
		&z_key_hash, &get_record, &z_filter, &z_read_policy) != SUCCESS) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters to Aerospike::get");
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zval_dtor(get_record);
	ZVAL_NULL(get_record);

	/* Load the default policies and merge them with any passed to the function */
	as_status converted = zval_to_as_policy_read(z_read_policy, &read_policy, &read_policy_p, &as_ptr->config.policies.read);
	if (converted != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid read read_policy");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);

	}

	if (z_hashtable_to_as_key(z_key_hash, &key, &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid key");
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	key_initialized = true;

	// Setup the return value
	// If a list of bins is passed, we will internally call aerospike_key_select,
	// else it will call aerospike_key_get
	if (z_filter) {

		if (Z_TYPE_P(z_filter) != IS_ARRAY) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Filter bins must be an array if provided");
			RETURN_LONG(AEROSPIKE_ERR_PARAM);		
		}
		z_filter_bins = Z_ARRVAL_P(z_filter);

		as_get_with_bins(as_ptr, &err, &key, read_policy_p, z_filter_bins, &record);
		if (err.code != AEROSPIKE_OK) {
			goto CLEANUP;
		}

	/* Call get if there were no bins passed in */
	} else {
	    aerospike_key_get(as_ptr, &err, read_policy_p, &key, &record);
	    if (err.code != AEROSPIKE_OK) {
			goto CLEANUP;
	    }
	}

	/* Handle showing the primary key on return */
	bool show_pk = false;
	show_pk = (read_policy_p && read_policy_p->key == AS_POLICY_KEY_SEND);
	as_record_to_zval(record, get_record, &key, show_pk, &err);


CLEANUP:

	if (key_initialized) {
		as_key_destroy(&key);
	}
	if (record) {
		as_record_destroy(record);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message);
		zval_dtor(get_record);
		array_init(get_record);
	}
	RETURN_LONG(err.code);
}
/* }}} */

/*
 * Helper function to handle a php array of bin names, and call aerospike_key_select
 */
static as_status as_get_with_bins(aerospike* as, as_error* err, const as_key* key, as_policy_read* read_policy_p,
		HashTable* bins, as_record** record) {

	char** c_filter_bins = NULL;
	int num_bins = 0;

	if (!hashtable_is_list(bins)) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Bins must be a list");
		return err->code;
	}

	num_bins = zend_hash_num_elements(bins);
	/** Array of strings must be null terminated,
	 ** so leave space for a null final entry.
	**/
	c_filter_bins = (char**)alloca((num_bins + 1) * sizeof(char*));
	c_filter_bins[num_bins] = NULL;

	int i = 0;
	zval* bin_val = NULL;

	ZEND_HASH_FOREACH_VAL(bins, bin_val)
	{
		/* If the value isn't a string bail out, need string values only */
		if (Z_TYPE_P(bin_val) != IS_STRING) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Bin names must be strings");
			return err->code;
		}

		if (Z_STRLEN_P(bin_val) > AS_BIN_NAME_MAX_LEN) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Bin name too long");
			return err->code;
		}

		/* Allocate and load the bin name */
		c_filter_bins[i] = (char*)alloca(AS_BIN_NAME_MAX_SIZE * sizeof(char));
		strncpy(c_filter_bins[i], Z_STRVAL_P(bin_val), AS_BIN_NAME_MAX_LEN);
		c_filter_bins[i][AS_BIN_NAME_MAX_LEN] = '\0';

		i++;
	} ZEND_HASH_FOREACH_END();

	aerospike_key_select(as, err, read_policy_p, key, (const char**)c_filter_bins, record);
	return err->code;
}
