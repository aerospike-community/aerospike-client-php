/*policy_conversions.c*/

#include "policy_conversions.h"
#include "php_ini.h"
#include "php_aerospike.h"
#include "php_aerospike_types.h"

/* Static functions */

static as_status set_base_policy_from_hash(HashTable* z_policy_hash, as_policy_base* base_policy);

static inline void set_uint32t_policy_value_from_hash_index(HashTable* z_policy_hash, uint32_t* target, int policy_index);
static inline void set_uint32t_policy_value_from_hash_key(HashTable* z_policy_hash, uint32_t* target, const char* policy_key);

static inline void set_bool_policy_value_from_hash_index(HashTable* z_policy_hash, bool* target, int policy_index);
static inline void set_bool_policy_value_from_hash_key(HashTable* z_policy_hash, bool* target, const char* policy_key );
static inline void set_key_policy_value_from_hash(HashTable* z_policy_hash, as_policy_key* target);
static inline void set_replica_policy_value_from_hash(HashTable* z_policy_hash, as_policy_replica* target);
static inline void set_read_mode_ap_policy_value_from_hash(HashTable* z_policy_hash, as_policy_read_mode_ap* target);
static inline void set_read_mode_sc_policy_value_from_hash(HashTable* z_policy_hash, as_policy_read_mode_sc* target);
static inline void set_exists_policy_value_from_hash(HashTable* z_policy_hash, as_policy_exists* target);
static inline void set_commit_level_policy_value_from_hash(HashTable* z_policy_hash, as_policy_commit_level* target);

// as_policy_read_mode_ap read_mode_ap
// bool 	deserialize
// as_policy_key 	key
// as_policy_replica 	replica
// uint32_t 	retry
// bool 	retry_on_timeout
// uint32_t 	sleep_between_retries
// uint32_t 	timeout

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

	setting_val = zend_hash_index_find(z_policy_hash, OPT_READ_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		read_policy->base.total_timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}
	set_read_policy_from_hash(z_policy_hash, read_policy);
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


	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		remove_policy->base.total_timeout = (uint32_t)Z_LVAL_P(setting_val);
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

	set_remove_policy_from_hash(z_policy_hash, remove_policy);
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
		write_policy->base.total_timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_GEN);
	if (setting_val && Z_TYPE_P(setting_val) == IS_ARRAY) {
		HashTable * gen_ary = Z_ARRVAL_P(setting_val);
		if (zend_hash_num_elements(gen_ary) > 0) {
			zval* value_code = NULL;
			value_code = zend_hash_index_find(gen_ary, 0);
			if (value_code && Z_TYPE_P(value_code) == IS_LONG) {
				write_policy->gen = (as_policy_gen)Z_LVAL_P(value_code);
			}
		}
		setting_val = NULL;
	}

	set_write_policy_from_hash(z_policy_hash, write_policy);

	return AEROSPIKE_OK;
}


//as_policy_operate
// as_policy_commit_level 	commit_level
// as_policy_read_mode_ap read_mode_ap
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

	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_GEN);
	if (setting_val && Z_TYPE_P(setting_val) == IS_ARRAY) {
		HashTable * gen_ary = Z_ARRVAL_P(setting_val);
		zval* value_code = NULL;
		// Take the first element of the ary
		value_code = zend_hash_index_find(gen_ary, 0);
		if (value_code) {
			operate_policy->gen = (as_policy_gen)Z_LVAL_P(value_code);
		}
		setting_val = NULL;
	}

	setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		operate_policy->base.total_timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	set_operate_policy_from_hash(z_policy_hash, operate_policy);
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
		apply_policy->base.total_timeout = (uint32_t)Z_LVAL_P(setting_val);
		setting_val = NULL;
	}

	set_apply_policy_from_hash(z_policy_hash, apply_policy);
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
			scan_policy->base.total_timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	//Check for write timeout
	} else {
		setting_val = zend_hash_index_find(z_policy_hash, OPT_WRITE_TIMEOUT);
		if (setting_val) {
			if (Z_TYPE_P(setting_val) == IS_LONG) {
				scan_policy->base.total_timeout = Z_LVAL_P(setting_val);
			} else {
				return AEROSPIKE_ERR_PARAM;
			}
		}
	}

	// RPS limit provided by the client for scan operations.
	setting_val = zend_hash_index_find(z_policy_hash, OPT_SCAN_RPS_LIMIT);
	if (setting_val) {
		if (Z_TYPE_P(setting_val) == IS_LONG) {
            scan_policy->records_per_second = Z_LVAL_P(setting_val);
        } else {
            return AEROSPIKE_ERR_PARAM;
        }
        setting_val = NULL;
	}

	set_scan_policy_from_hash(z_policy_hash, scan_policy);

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
			query_policy->base.total_timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

	set_query_policy_from_hash(z_policy_hash, query_policy);

	return AEROSPIKE_OK;
}

as_status set_query_options_from_policy_hash(as_query* query, zval* z_policy) {

	zval* nobins_val = NULL;
	HashTable* z_policy_hash = NULL;

	if (!z_policy ||  Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	z_policy_hash = Z_ARRVAL_P(z_policy);
	nobins_val = zend_hash_index_find(z_policy_hash, OPT_QUERY_NOBINS);

	if (nobins_val) {
		if (Z_TYPE_P(nobins_val) == IS_TRUE || Z_TYPE_P(nobins_val) == IS_FALSE ) {
			query->no_bins = (zend_bool)Z_LVAL_P(nobins_val);
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
			batch_policy->base.total_timeout = Z_LVAL_P(setting_val);
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
		setting_val = NULL;
	}

	set_batch_policy_from_hash(z_policy_hash, batch_policy);

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
// as_policy_read_mode_ap read_mode_ap
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

as_status
zval_to_as_policy_map(zval* z_policy, as_map_policy* map_policy) {

	HashTable* policy_hash = NULL;
	as_map_policy_init(map_policy);
	long map_order = AS_MAP_UNORDERED;
	long write_mode = AS_MAP_UPDATE;
	long write_flags = AS_MAP_WRITE_DEFAULT;

	if (!z_policy || Z_TYPE_P(z_policy) == IS_NULL) {
		return AEROSPIKE_OK;
	}

	if (Z_TYPE_P(z_policy) != IS_ARRAY) {
		return AEROSPIKE_ERR_PARAM;
	}

	policy_hash = Z_ARRVAL_P(z_policy);

	zval* map_policy_val = NULL;
	zval* map_policy_flags_val = NULL;

	map_policy_val = zend_hash_index_find(policy_hash, OPT_MAP_ORDER);
	if (map_policy_val) {
		if (Z_TYPE_P(map_policy_val) == IS_LONG) {
			map_order = Z_LVAL_P(map_policy_val);
			if (map_order != AS_MAP_UNORDERED &&
				map_order != AS_MAP_KEY_ORDERED &&
				map_order != AS_MAP_KEY_VALUE_ORDERED) {
				return AEROSPIKE_ERR_PARAM;
			}
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
	}

	map_policy_flags_val = zend_hash_index_find(policy_hash, OPT_MAP_WRITE_FLAGS);
	if (map_policy_flags_val) {
		if (Z_TYPE_P(map_policy_flags_val) == IS_LONG) {
			write_flags = Z_LVAL_P(map_policy_flags_val);
			long write_flags_chk = write_flags & ~AS_MAP_WRITE_NO_FAIL &
									~AS_MAP_WRITE_PARTIAL;
			if (write_flags_chk != AS_MAP_WRITE_DEFAULT &&
				write_flags_chk != AS_MAP_WRITE_CREATE_ONLY &&
				write_flags_chk != AS_MAP_WRITE_UPDATE_ONLY) {
				return AEROSPIKE_ERR_PARAM;
			}
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
	}

	map_policy_val = zend_hash_index_find(policy_hash, OPT_MAP_WRITE_MODE);
	if (map_policy_val) {
		if (Z_TYPE_P(map_policy_val) == IS_LONG) {
			write_mode = Z_LVAL_P(map_policy_val);
			if (write_mode != AS_MAP_UPDATE &&
				write_mode != AS_MAP_UPDATE_ONLY &&
				write_mode != AS_MAP_CREATE_ONLY) {
				return AEROSPIKE_ERR_PARAM;
			}
		} else {
			return AEROSPIKE_ERR_PARAM;
		}
	}

	if (map_policy_flags_val)
		as_map_policy_set_flags(map_policy, map_order, write_flags);
	else
		as_map_policy_set(map_policy, map_order, write_mode);

	return AEROSPIKE_OK;
}

as_status set_read_policy_from_hash(HashTable* z_policy_hash, as_policy_read* read_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_read_mode_ap_policy_value_from_hash(z_policy_hash, &read_policy->read_mode_ap);
	set_bool_policy_value_from_hash_key(z_policy_hash, &read_policy->deserialize, PHP_POLICY_OPT_DESERIALIZE);
	set_read_mode_sc_policy_value_from_hash(z_policy_hash, &read_policy->read_mode_sc);
	set_key_policy_value_from_hash(z_policy_hash, &read_policy->key);
	set_replica_policy_value_from_hash(z_policy_hash, &read_policy->replica);

	set_base_policy_from_hash(z_policy_hash, &read_policy->base);
	return AEROSPIKE_OK;
}

as_status set_write_policy_from_hash(HashTable* z_policy_hash, as_policy_write* write_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_uint32t_policy_value_from_hash_index(z_policy_hash, &write_policy->compression_threshold, COMPRESSION_THRESHOLD);
	set_exists_policy_value_from_hash(z_policy_hash, &write_policy->exists);
	set_commit_level_policy_value_from_hash(z_policy_hash, &write_policy->commit_level);
	set_bool_policy_value_from_hash_index(z_policy_hash, &write_policy->durable_delete, OPT_POLICY_DURABLE_DELETE);
	set_key_policy_value_from_hash(z_policy_hash, &write_policy->key);
	set_replica_policy_value_from_hash(z_policy_hash, &write_policy->replica);

	set_base_policy_from_hash(z_policy_hash, &write_policy->base);
	return AEROSPIKE_OK;
}

as_status set_remove_policy_from_hash(HashTable* z_policy_hash, as_policy_remove* remove_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_commit_level_policy_value_from_hash(z_policy_hash, &remove_policy->commit_level);
	set_bool_policy_value_from_hash_index(z_policy_hash, &remove_policy->durable_delete, OPT_POLICY_DURABLE_DELETE);
	set_key_policy_value_from_hash(z_policy_hash, &remove_policy->key);
	set_replica_policy_value_from_hash(z_policy_hash, &remove_policy->replica);

	set_base_policy_from_hash(z_policy_hash, &remove_policy->base);
	return AEROSPIKE_OK;
}

as_status set_batch_policy_from_hash(HashTable* z_policy_hash, as_policy_batch* batch_policy) {


	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_bool_policy_value_from_hash_index(z_policy_hash, &batch_policy->concurrent, OPT_BATCH_CONCURRENT);
	set_bool_policy_value_from_hash_index(z_policy_hash, &batch_policy->allow_inline, OPT_ALLOW_INLINE);
	set_bool_policy_value_from_hash_index(z_policy_hash, &batch_policy->send_set_name, OPT_SEND_SET_NAME);
	set_read_mode_ap_policy_value_from_hash(z_policy_hash, &batch_policy->read_mode_ap);
	set_bool_policy_value_from_hash_key(z_policy_hash, &batch_policy->deserialize, PHP_POLICY_OPT_DESERIALIZE);
	set_read_mode_sc_policy_value_from_hash(z_policy_hash, &batch_policy->read_mode_sc);

	set_base_policy_from_hash(z_policy_hash, &batch_policy->base);
	return AEROSPIKE_OK;
}

as_status set_operate_policy_from_hash(HashTable* z_policy_hash, as_policy_operate* operate_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_read_mode_ap_policy_value_from_hash(z_policy_hash, &operate_policy->read_mode_ap);
	set_bool_policy_value_from_hash_key(z_policy_hash, &operate_policy->deserialize, PHP_POLICY_OPT_DESERIALIZE);
	set_read_mode_sc_policy_value_from_hash(z_policy_hash, &operate_policy->read_mode_sc);
	set_exists_policy_value_from_hash(z_policy_hash, &operate_policy->exists);
	set_commit_level_policy_value_from_hash(z_policy_hash, &operate_policy->commit_level);
	set_bool_policy_value_from_hash_index(z_policy_hash, &operate_policy->durable_delete, OPT_POLICY_DURABLE_DELETE);
	set_key_policy_value_from_hash(z_policy_hash, &operate_policy->key);
	set_replica_policy_value_from_hash(z_policy_hash, &operate_policy->replica);

	set_base_policy_from_hash(z_policy_hash, &operate_policy->base);
	return AEROSPIKE_OK;
}

as_status set_query_policy_from_hash(HashTable* z_policy_hash, as_policy_query* query_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_bool_policy_value_from_hash_key(z_policy_hash, &query_policy->deserialize, PHP_POLICY_OPT_DESERIALIZE);
	set_base_policy_from_hash(z_policy_hash, &query_policy->base);
	return AEROSPIKE_OK;
}

as_status set_scan_policy_from_hash(HashTable* z_policy_hash, as_policy_scan* scan_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_bool_policy_value_from_hash_index(z_policy_hash, &scan_policy->durable_delete, OPT_POLICY_DURABLE_DELETE);
	set_bool_policy_value_from_hash_index(z_policy_hash, &scan_policy->fail_on_cluster_change, OPT_FAIL_ON_CLUSTER_CHANGE);
	set_uint32t_policy_value_from_hash_index(z_policy_hash, &scan_policy->records_per_second, OPT_SCAN_RPS_LIMIT);
	set_base_policy_from_hash(z_policy_hash, &scan_policy->base);
	return AEROSPIKE_OK;
}

as_status set_apply_policy_from_hash(HashTable* z_policy_hash, as_policy_apply* apply_policy) {

	if (!z_policy_hash) {
		return AEROSPIKE_OK;
	}

	set_commit_level_policy_value_from_hash(z_policy_hash, &apply_policy->commit_level);
	set_bool_policy_value_from_hash_index(z_policy_hash, &apply_policy->durable_delete, OPT_POLICY_DURABLE_DELETE);
	set_key_policy_value_from_hash(z_policy_hash, &apply_policy->key);
	set_replica_policy_value_from_hash(z_policy_hash, &apply_policy->replica);

	set_base_policy_from_hash(z_policy_hash, &apply_policy->base);
	return AEROSPIKE_OK;
}

static as_status set_base_policy_from_hash(HashTable* z_policy_hash, as_policy_base* base_policy) {
	set_uint32t_policy_value_from_hash_key(z_policy_hash, &base_policy->sleep_between_retries, PHP_POLICY_OPT_SLEEP_BETWEEN_RETRIES);
	set_uint32t_policy_value_from_hash_index(z_policy_hash, &base_policy->max_retries, OPT_MAX_RETRIES);
	set_uint32t_policy_value_from_hash_index(z_policy_hash, &base_policy->total_timeout, OPT_TOTAL_TIMEOUT);
	set_uint32t_policy_value_from_hash_index(z_policy_hash, &base_policy->socket_timeout, OPT_SOCKET_TIMEOUT);

	return AEROSPIKE_OK;
}

static inline void set_uint32t_policy_value_from_hash_index(HashTable* z_policy_hash, uint32_t* target, int policy_index) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, policy_index);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (uint32_t)Z_LVAL_P(setting_val);
	}
}

static inline void set_uint32t_policy_value_from_hash_key(HashTable* z_policy_hash, uint32_t* target, const char* policy_key) {
	zval* setting_val = NULL;
	setting_val = zend_hash_str_find(z_policy_hash, policy_key, strlen(policy_key));
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (uint32_t)Z_LVAL_P(setting_val);
	}
}

static inline void set_bool_policy_value_from_hash_index(HashTable* z_policy_hash, bool* target, int policy_index) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, policy_index);
	if (setting_val && ((Z_TYPE_P(setting_val) == IS_TRUE) || (Z_TYPE_P(setting_val) == IS_FALSE)) ) {
		*target = (Z_TYPE_P(setting_val) == IS_TRUE);
	}
}

static inline void set_bool_policy_value_from_hash_key(HashTable* z_policy_hash, bool* target, const char* policy_key ) {
	zval* setting_val = NULL;
	setting_val = zend_hash_str_find(z_policy_hash, policy_key, strlen(policy_key));
	if (setting_val && ((Z_TYPE_P(setting_val) == IS_TRUE) || (Z_TYPE_P(setting_val) == IS_FALSE)) ) {
		*target = (Z_TYPE_P(setting_val) == IS_TRUE);
	}
}

static inline void set_key_policy_value_from_hash(HashTable* z_policy_hash, as_policy_key* target) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_KEY);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (as_policy_key)Z_LVAL_P(setting_val);
	}
}

static inline void set_replica_policy_value_from_hash(HashTable* z_policy_hash, as_policy_replica* target) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_REPLICA);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (as_policy_replica)Z_LVAL_P(setting_val);
	}
}

static inline void set_read_mode_ap_policy_value_from_hash(HashTable* z_policy_hash, as_policy_read_mode_ap* target) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_READ_MODE_AP);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (as_policy_read_mode_ap)Z_LVAL_P(setting_val);
	}
}

static inline void set_read_mode_sc_policy_value_from_hash(HashTable* z_policy_hash, as_policy_read_mode_sc* target) {
    zval* setting_val = NULL;
    setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_READ_MODE_SC);
    if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
        *target = (as_policy_read_mode_sc)Z_LVAL_P(setting_val);
    }
}

static inline void set_exists_policy_value_from_hash(HashTable* z_policy_hash, as_policy_exists* target) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_EXISTS);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (as_policy_exists)Z_LVAL_P(setting_val);
	}
}

static inline void set_commit_level_policy_value_from_hash(HashTable* z_policy_hash, as_policy_commit_level* target) {
	zval* setting_val = NULL;
	setting_val = zend_hash_index_find(z_policy_hash, OPT_POLICY_COMMIT_LEVEL);
	if (setting_val && Z_TYPE_P(setting_val) == IS_LONG) {
		*target = (as_policy_commit_level)Z_LVAL_P(setting_val);
	}
}
