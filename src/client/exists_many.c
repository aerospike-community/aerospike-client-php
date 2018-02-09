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
#include "aerospike_class.h"
#include "policy_conversions.h"
#include "php_aerospike_types.h"
#include "aerospike/aerospike_batch.h"
#include "conversions.h"

typedef struct _exists_many_cb_data {
	as_error* err;
	zval* metadata_ary;
}exists_many_cb_data;

/*
 * this will build up an array of the form.
 * [
 * 	[
 * 		"key" => ["ns"=>.., "set"=>.., "key"=>.., "digest"=>..],
 * 		"metadata" => ["generation"=>.., "ttl"=>..]
 * 	],..
 *
 * ]
 * On failure an err will be set, and false will be returned
 */
bool exists_callback(const as_batch_read * results, uint32_t n, void * udata) {
	zval exists_entry;
	zval meta_entry;
	const as_key* key = NULL;
	zval key_entry;

	as_batch_read read_result;
	exists_many_cb_data* callback_data = (exists_many_cb_data*)udata;

	for (int i = 0; i < n; i++) {
		read_result = results[i];

		if ( (read_result.result != AEROSPIKE_OK) && (read_result.result != AEROSPIKE_ERR_RECORD_NOT_FOUND) ) {
			as_error_update(callback_data->err, AEROSPIKE_ERR_CLIENT, "existsMany failed");
			return false;
		}

		key = read_result.key;
		if (!key) {
			as_error_update(callback_data->err, AEROSPIKE_ERR_CLIENT, "existsMany returned a null key");
			return false;
		}

		if (as_key_to_zval(key, &key_entry, true, callback_data->err) != AEROSPIKE_OK) {
			return false;
		}

		/* In the case of record not found, we insert a NULL for metadata */
		ZVAL_NULL(&meta_entry);
		if (read_result.result == AEROSPIKE_OK) {
			array_init(&meta_entry);
			if (as_record_to_z_metadata(
					&read_result.record, &meta_entry, callback_data->err) != AEROSPIKE_OK) {
				/* Because we previously allocated the key, we need to free it here */
				zval_dtor(&key_entry);
				zval_dtor(&meta_entry);
				return false;
			}
		}

		array_init(&exists_entry);

		/* key_entry and meta_entry never get dtor'd so we don't need to increment
		 * their reference count */
		add_assoc_zval(&exists_entry, "key", &key_entry);
		add_assoc_zval(&exists_entry, "metadata", &meta_entry);
		add_next_index_zval(callback_data->metadata_ary, &exists_entry);
	}
    return true;
}


/* {{{ proto int Aerospike::existsMany( array keys, array &metadata [, array options] )
   Returns metadata for a batch of records with NULL for non-existent ones */
PHP_METHOD(Aerospike, existsMany) {
	as_error err;
	as_error_init(&err);

	aerospike* as_client;
	AerospikeClient* php_client;
	HashTable* z_key_array = NULL;
	/* zval by reference to be filled with results */
	zval* z_metadata = NULL;
	zval* z_policy = NULL;

	as_batch batch;
	bool batch_initialized = false;

	uint32_t key_count;
	as_policy_batch batch_policy;
	as_policy_batch* batch_policy_p = NULL;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "hz/|z",
			&z_key_array, &z_metadata, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid arguments to existsMany");
		goto CLEANUP;
	}
	zval_dtor(z_metadata);
	ZVAL_NULL(z_metadata);

	/* Set the batch policy */
	if (z_policy) {
		if (zval_to_as_policy_batch(z_policy, &batch_policy,
				&batch_policy_p, &as_client->config.policies.batch) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid batch policy");
			goto CLEANUP;
		}
		batch_policy_p = &batch_policy;
	}

	key_count = zend_hash_num_elements(z_key_array);

	as_batch_init(&batch, key_count);
	batch_initialized = true;
	int i = 0;
	zval* current_key = NULL;

	/* Load the php keys, into the as_batch structure */
	ZEND_HASH_FOREACH_VAL(z_key_array, current_key) {
		if (Z_TYPE_P(current_key) != IS_ARRAY) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid Key");
			goto CLEANUP;
		}
		if (z_hashtable_to_as_key(Z_ARRVAL_P(current_key), as_batch_keyat(&batch, i), &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
		i++;
	}ZEND_HASH_FOREACH_END();

	array_init(z_metadata);
	exists_many_cb_data cb_data;
	cb_data.err = &err;
	cb_data.metadata_ary = z_metadata;

	/* Setup complete, run the actual batch_exists function */
	aerospike_batch_exists(as_client, &err, batch_policy_p, &batch,
			(aerospike_batch_read_callback)exists_callback, (void*)&cb_data);

CLEANUP:
	if (batch_initialized) {
		as_batch_destroy(&batch);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);

		/* If this was created, and is an array, we need to clear it */
		if (z_metadata && Z_TYPE_P(z_metadata) == IS_ARRAY) {
			zval_dtor(z_metadata);
			ZVAL_NULL(z_metadata);
		}
	}

	RETURN_LONG(err.code);
}
/* }}} */
