/*policy_conversions.c*/

#include "policy_conversions.h"
#include "php_ini.h"
#include "php_aerospike.h"
#include "php_aerospike_types.h"

// as_policy_consistency_level 	consistency_level
// bool 	deserialize
// as_policy_key 	key
// as_policy_replica 	replica
// uint32_t 	retry
// bool 	retry_on_timeout
// uint32_t 	sleep_between_retries
// uint32_t 	timeout

/* fix to actually load ini vals */
as_status zval_to_as_policy_read(zval* z_policy, as_policy_read* read_policy,
		as_policy_read** read_policy_p, as_policy_read* default_policy) {
	as_policy_read_init(read_policy);

	HashTable* z_policy_hash = NULL;

	as_policy_read_copy(default_policy, read_policy);
	*read_policy_p = read_policy;

	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}
	zval* setting_val = NULL;

	z_policy_hash = Z_ARRVAL_P(z_policy);

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_CONSISTENCY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->consistency_level = (as_policy_consistency_level)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	// deserialize policy
	setting_val = zend_hash_str_find(z_policy_hash, "deserialize", strlen("deserialize"));
	if (setting_val && ((Z_TYPE_P(setting_val) == IS_TRUE) || (Z_TYPE_P(setting_val) == IS_FALSE ))) {
		read_policy->deserialize = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}
	// key_policy
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_KEY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->key = (as_policy_key)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}
	// replica _policy
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_REPLICA);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->replica = (as_policy_replica)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_RETRY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->retry = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "retry_on_timeout", strlen("retry_on_timeout"));
	if (setting_val && ((Z_TYPE_P(setting_val) == IS_TRUE) || (Z_TYPE_P(setting_val) == IS_FALSE ))) {
		read_policy->retry_on_timeout = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "sleep_between_retries", strlen("sleep_between_retries"));
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->sleep_between_retries = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_READ_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	return AEROSPIKE_OK;
}


// as_policy_commit_level 	commit_level
// bool 	durable_delete
// as_policy_gen 	gen
// uint16_t 	generation
// as_policy_key 	key
// uint32_t 	retry
// bool 	retry_on_timeout
// uint32_t 	sleep_between_retries
// uint32_t 	timeout

as_status zval_to_as_policy_remove(zval* z_policy, as_policy_remove* remove_policy,
								   as_policy_remove** remove_policy_p, as_policy_remove* default_policy)
{
	as_policy_remove_init(remove_policy);

	as_policy_remove_copy(default_policy, remove_policy);
	*remove_policy_p = remove_policy;

	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	HashTable* z_policy_hash = NULL;
	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_policy);
	zval* setting_val = NULL;

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_COMMIT_LEVEL);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		remove_policy->commit_level = (as_policy_commit_level)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_GEN);
	if (setting_val && Z_TYPE_P(setting_val) == IS_ARRAY) {
		HashTable * gen_ary = Z_ARRVAL_P(setting_val);
		zval* value_code = NULL;
		if (zend_hash_num_elements(gen_ary) == 2) {
			value_code = zend_hash_index_find(gen_ary, 0);
			remove_policy->gen = (as_policy_gen)Z_LVAL_P(value_code);

			value_code = zend_hash_index_find(gen_ary, 1);
			if (Z_TYPE_P(value_code) != IS_LONG) {
				return AEROSPIKE_ERR_PARAM; // This is an invalid generation
			}
			remove_policy->generation = (uint16_t)Z_LVAL_P(value_code);
		}
		setting_val = NULL;

	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_KEY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		remove_policy->key = (as_policy_key)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_RETRY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		remove_policy->retry = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}


	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		remove_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_DURABLE_DELETE);
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		remove_policy->durable_delete = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	return AEROSPIKE_OK;

}


// uint32_t timeout;
// uint32_t retry;
// uint32_t sleep_between_retries;
// uint32_t compression_threshold;
// as_policy_key key;
// as_policy_gen gen;
// as_policy_exists exists;
// as_policy_commit_level commit_level;
// bool retry_on_timeout;
// bool durable_delete;
as_status zval_to_as_policy_write(zval* z_policy, as_policy_write* write_policy,
								  as_policy_write** write_policy_p, as_policy_write* default_policy) {

	as_policy_write_init(write_policy);
	as_policy_write_copy(default_policy, write_policy);
	*write_policy_p = write_policy;

	HashTable* z_policy_hash = NULL;

	// IF null was explicitly passed, just return the defaults
	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}
	zval* setting_val = NULL;

	z_policy_hash = Z_ARRVAL_P(z_policy);

	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_RETRY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->retry = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "sleep_between_retries", strlen("sleep_between_retries"));
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->sleep_between_retries = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, COMPRESSION_THRESHOLD);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->compression_threshold = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_KEY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->key = (as_policy_key)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_GEN);
	if (setting_val && Z_TYPE_P(setting_val) == IS_ARRAY) {
		HashTable * gen_ary = Z_ARRVAL_P(setting_val);
		zval* value_code = NULL;
		value_code = zend_hash_index_find(gen_ary, 0);
		write_policy->gen = (as_policy_gen)Z_LVAL_P(value_code);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_EXISTS);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->exists = (as_policy_exists)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_COMMIT_LEVEL);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		write_policy->commit_level = (as_policy_commit_level)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "retry_on_timeout", strlen("retry_on_timeout"));
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		write_policy->retry_on_timeout = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_DURABLE_DELETE);
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		write_policy->durable_delete = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	return AEROSPIKE_OK;
}


//as_policy_operate
// as_policy_commit_level 	commit_level
// as_policy_consistency_level 	consistency_level
// bool 	deserialize
// bool 	durable_delete
// as_policy_gen 	gen
// as_policy_key 	key
// as_policy_replica 	replica
// uint32_t 	retry
// bool 	retry_on_timeout
// uint32_t 	sleep_between_retries
// uint32_t 	timeout

as_status zval_to_as_policy_operate(zval* z_policy, as_policy_operate* operate_policy,
									as_policy_operate** operate_policy_p, as_policy_operate* default_policy) {

	as_policy_operate_init(operate_policy);

	as_policy_operate_copy(operate_policy, default_policy);
	*operate_policy_p = operate_policy;

	if (!z_policy) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	HashTable* z_policy_hash = NULL;
	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}


	z_policy_hash = Z_ARRVAL_P(z_policy);
	zval* setting_val = NULL;

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_COMMIT_LEVEL);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->commit_level = (as_policy_commit_level)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_CONSISTENCY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->consistency_level = (as_policy_consistency_level)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "deserialize", strlen("deserialize"));
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		operate_policy->deserialize = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_DURABLE_DELETE);
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		operate_policy->durable_delete = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_GEN);
	if (setting_val && Z_TYPE_P(setting_val) == IS_ARRAY) {
		HashTable * gen_ary = Z_ARRVAL_P(setting_val);
		zval* value_code = NULL;
		// Take the first element of the ary
		value_code = zend_hash_index_find(gen_ary, 0);
		operate_policy->gen = (as_policy_gen)Z_LVAL_P(value_code);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_KEY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->key = (as_policy_key)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_REPLICA);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->replica = (as_policy_replica)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_RETRY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->retry = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "retry_on_timeout", strlen("retry_on_timeout"));
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		operate_policy->retry_on_timeout = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	setting_val = zend_hash_str_find(z_policy_hash, "sleep_between_retries", strlen("sleep_between_retries"));
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->sleep_between_retries = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	return AEROSPIKE_OK;
}

as_status zval_to_as_policy_info(zval* z_info_policy, as_policy_info* info_policy,
								 as_policy_info** info_policy_p, as_policy_info* default_policy) {
	zval* setting_val;
	HashTable* z_policy_hash;
	as_policy_info_init(info_policy);

	as_policy_info_copy(default_policy, info_policy);
	*info_policy_p = info_policy;

	if (!z_info_policy || (Z_TYPE_P(z_info_policy) == IS_NULL)) {
		return AEROSPIKE_OK;
	}
	if (Z_TYPE_P(z_info_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_info_policy);
	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		info_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_READ_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		info_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
	}

	return AEROSPIKE_OK;
}

as_status zval_to_as_policy_apply(zval* z_apply_policy, as_policy_apply* apply_policy,
								  as_policy_apply** apply_policy_p, as_policy_apply* default_policy) {
	zval* setting_val;
	HashTable* z_policy_hash;

	as_policy_apply_init(apply_policy);
	as_policy_apply_copy(default_policy, apply_policy);
	*apply_policy_p = apply_policy;

	if (!z_apply_policy || (Z_TYPE_P(z_apply_policy) == IS_NULL)) {
		return AEROSPIKE_OK;
	}
	if (Z_TYPE_P(z_apply_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_apply_policy);
	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		apply_policy->timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_KEY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		apply_policy->key = (as_policy_key)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_DURABLE_DELETE);
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		apply_policy->durable_delete = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	return AEROSPIKE_OK;
}

as_status zval_to_as_policy_scan(zval* z_policy, as_policy_scan* scan_policy,
		as_policy_scan** scan_policy_p, as_policy_scan* default_policy) {

	as_policy_scan_init(scan_policy);
	as_policy_scan_copy(default_policy, scan_policy);

	*scan_policy_p = scan_policy;

	HashTable* z_policy_hash = NULL;

	// IF null was explicitly passed, just return the defaults
	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_policy);
	zval* setting_val = NULL;


	// Write timeout is provided to scanApply, so we need to check for it as well
	// as read. If read and write timeout are provided, read will be used
	// Both map to the timeout field anyway, so that isn't a big deal
	setting_val = zend_hash_index_find(z_policy_hash, OPT_READ_TIMEOUT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			scan_policy->timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	//Check for write timeout
	} else {
		setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
		if (setting_val) {
			if (Z_TYPE_P(setting_val) == IS_LONG) {
				scan_policy->timeout = Z_LVAL_P(setting_val);
			} else {
				return AEROSPIKE_ERR_PARAM;
			}
		}
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_SOCKET_TIMEOUT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			scan_policy->socket_timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_DURABLE_DELETE);
	if (setting_val && (Z_TYPE_P(setting_val) == IS_TRUE || Z_TYPE_P(setting_val) == IS_FALSE )) {
		scan_policy->durable_delete = Z_TYPE_P(setting_val) == IS_TRUE ? true : false;
		setting_val = NULL;
	}

	return AEROSPIKE_OK;
}

as_status zval_to_as_policy_query(zval* z_policy, as_policy_query* query_policy,
								  as_policy_query** query_policy_p, as_policy_query* default_policy) {

	as_policy_query_init(query_policy);
	as_policy_query_copy(default_policy, query_policy);
	*query_policy_p = query_policy;

	HashTable* z_policy_hash = NULL;

	// IF null was explicitly passed, just return the defaults
	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_policy);
	zval* setting_val = NULL;


	setting_val = zend_hash_index_find(z_policy_hash, OPT_READ_TIMEOUT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			query_policy->timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_SOCKET_TIMEOUT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			query_policy->socket_timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
	}

	return AEROSPIKE_OK;
}

as_status zval_to_as_policy_batch(zval* z_policy, as_policy_batch* batch_policy,
								  as_policy_batch** batch_policy_p, as_policy_batch* default_policy) {
	as_policy_batch_init(batch_policy);
	as_policy_batch_copy(default_policy, batch_policy);
	*batch_policy_p = batch_policy;

	HashTable* z_policy_hash = NULL;

	// IF null was explicitly passed, just return the defaults
	if (!z_policy || (Z_TYPE_P(z_policy) == IS_NULL) ) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		// error out here wrong type
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_policy);
	zval* setting_val = NULL;


	setting_val = zend_hash_index_find(z_policy_hash, OPT_READ_TIMEOUT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			batch_policy->timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

	return AEROSPIKE_OK;
}

as_status zval_to_as_policy_admin(zval* z_policy, as_policy_admin* admin_policy,
								  as_policy_admin** admin_policy_p, as_policy_admin* default_policy) {

	as_policy_admin_init(admin_policy);
	as_policy_admin_copy(default_policy, admin_policy);
	*admin_policy_p = admin_policy;
	HashTable* z_policy_hash = NULL;

	if (!z_policy || (Z_TYPE_P(z_policy) == IS_NULL) ) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_policy);
	zval* setting_val = NULL;

	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			admin_policy->timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

	return AEROSPIKE_OK;

}

as_status set_scan_options_from_policy_hash(as_scan* scan, zval* z_policy) {

	HashTable* z_policy_ary = NULL;
	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	zval* setting_val = NULL;
	z_policy_ary = Z_ARRVAL_P(z_policy);

	setting_val = zend_hash_index_find(z_policy_ary, OPT_SCAN_PRIORITY);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			scan->priority = (as_scan_priority)Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_ary, OPT_SCAN_PERCENTAGE);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
			scan->percent = (uint8_t)Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

/* Concurrent scans run into issues with thread local heap allocations,
 * run by threads not created by php, only allow them when running
 * in non ZTS environment. Run this block all the time to catch invalid
 * options, but in ZTS it will ignore the actual value.
 */

	setting_val = zend_hash_index_find(z_policy_ary, OPT_SCAN_CONCURRENTLY);
	if (setting_val) {
		if ((Z_TYPE_P(setting_val) != IS_TRUE) && (Z_TYPE_P(setting_val) != IS_FALSE)) {
			return AEROSPIKE_ERR_PARAM;
		}

#ifndef ZTS
		if (Z_TYPE_P(setting_val) == IS_TRUE) {
			scan->concurrent = true;
		}
#endif

		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_ary, OPT_SCAN_NOBINS);
	if (setting_val) {

		if ((Z_TYPE_P(setting_val) != IS_TRUE) && (Z_TYPE_P(setting_val) != IS_FALSE)) {
			return AEROSPIKE_ERR_PARAM;
		}
		if (Z_TYPE_P(setting_val) == IS_TRUE) {
			scan->no_bins = true;
		}
	}
	/* Omitting LDT due to deprecation */

	return AEROSPIKE_OK;
}
//as_policy_operate
// as_policy_commit_level 	commit_level
// as_policy_consistency_level 	consistency_level
// bool 	deserialize
// bool 	durable_delete
// as_policy_gen 	gen
// as_policy_key 	key
// as_policy_replica 	replica
// uint32_t 	retry
// bool 	retry_on_timeout
// uint32_t 	sleep_between_retries
// uint32_t 	timeout

/* change this later*/
as_status set_record_generation_from_write_policy(as_record* record, zval* z_write_policy) {
	if (!z_write_policy || Z_TYPE_P(z_write_policy) != IS_ARRAY) {
		return AEROSPIKE_OK;
	}
	HashTable* write_ary = Z_ARRVAL_P(z_write_policy);
	zval* gen_zval = zend_hash_index_find(write_ary, OPT_POLICY_GEN);
	if (!gen_zval) {
		return AEROSPIKE_OK;
	}
	if (Z_TYPE_P(gen_zval) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM; //error
	}

	HashTable* gen_ary = Z_ARRVAL_P(gen_zval);
	
	if (zend_hash_num_elements(gen_ary) != 2) {
		return AEROSPIKE_OK; // No value provided, return
	}
	
	zval* generation_value = zend_hash_index_find(gen_ary, 1);

	if (!generation_value || Z_TYPE_P(generation_value) != IS_LONG) {
		return AEROSPIKE_ERR_PARAM; // error
	}

	record->gen = Z_LVAL_P(generation_value);

	return AEROSPIKE_OK;
}

as_status set_operations_generation_from_operate_policy(as_operations* operations, zval* z_op_policy) {
	if (!z_op_policy || Z_TYPE_P(z_op_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_op_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}
	HashTable* opt_ary = Z_ARRVAL_P(z_op_policy);
	zval* gen_zval = zend_hash_index_find(opt_ary, OPT_POLICY_GEN);
	if (!gen_zval) {
		return AEROSPIKE_OK;
	}
	if (Z_TYPE_P(gen_zval) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM; //error
	}

	HashTable* gen_ary = Z_ARRVAL_P(gen_zval);
	
	if (zend_hash_num_elements(gen_ary) != 2) {
		return AEROSPIKE_OK; // No value provided, return
	}
	
	zval* generation_value = zend_hash_index_find(gen_ary, 1);

	if (!generation_value || Z_TYPE_P(generation_value) != IS_LONG) {
		return AEROSPIKE_ERR_PARAM; // error
	}

	operations->gen = Z_LVAL_P(generation_value);

	return AEROSPIKE_OK;
}

/*
 * Look for the value of [AEROSPIKE::OPT_TTL => ####] and assign it to the operations ttl field
 * Return error if the value is not an integer
 */
as_status set_operations_ttl_from_operate_policy(as_operations* operations, zval* z_op_policy) {
	if (!z_op_policy || Z_TYPE_P(z_op_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_op_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	HashTable* opt_ary = Z_ARRVAL_P(z_op_policy);
	zval* ttl_zval = zend_hash_index_find(opt_ary, OPT_TTL);

	if (!ttl_zval) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(ttl_zval) != IS_LONG) {
		return AEROSPIKE_ERR_PARAM; //error
	}

	operations->ttl = (uint32_t)Z_LVAL_P(ttl_zval);

	return AEROSPIKE_OK;
}

as_status set_serializer_from_policy_hash(int* serializer_type, zval* z_policy) {
	HashTable* z_policy_ary = NULL;
	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		*serializer_type = INI_INT("aerospike.serializer");
		return AEROSPIKE_OK;
	}
	zval* z_serializer_type = NULL;
	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}
	z_policy_ary = Z_ARRVAL_P(z_policy);

	z_serializer_type = zend_hash_index_find(z_policy_ary, OPT_SERIALIZER);
	if (!z_serializer_type) {
		*serializer_type = INI_INT("aerospike.serializer");
		return AEROSPIKE_OK;
	}
	// invalid policy value
	if (Z_TYPE_P(z_serializer_type) != IS_LONG) {
		return AEROSPIKE_ERR_PARAM;
	}

	*serializer_type = Z_LVAL_P(z_serializer_type);
	return AEROSPIKE_OK;
}

as_status set_deserializer_from_policy_hash(int* deserializer_type, zval* z_policy) {
	HashTable* z_policy_ary = NULL;
	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		*deserializer_type = SERIALIZER_PHP;
		return AEROSPIKE_OK;
	}
	zval* z_serializer_type = NULL;
	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}
	z_policy_ary = Z_ARRVAL_P(z_policy);

	z_serializer_type = zend_hash_index_find(z_policy_ary, OPT_SERIALIZER);
	if (!z_serializer_type) {
		*deserializer_type = SERIALIZER_PHP;
		return AEROSPIKE_OK;
	}
	// invalid policy value
	if (Z_TYPE_P(z_serializer_type) != IS_LONG) {
		return AEROSPIKE_ERR_PARAM;
	}

	*deserializer_type = Z_LVAL_P(z_serializer_type);
	return AEROSPIKE_OK;
}
