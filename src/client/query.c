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

#include "conversions.h"
#include "aerospike/as_error.h"
#include "aerospike_class.h"
#include "php_aerospike_types.h"
#include "user_callbacks.h"
#include "aerospike/aerospike_query.h"
#include "policy_conversions.h"

#define QUERY_WHERE_OP_KEY "op"
#define QUERY_WHERE_VAL_KEY "val"
#define QUERY_WHERE_BIN_KEY "bin"
#define QUERY_WHERE_INDEX_TYPE_KEY "index_type"

as_status add_predicate_to_query(as_query* query, const HashTable* z_predicate_val, as_error* err);

as_status add_equal_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
as_status add_geo_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
as_status add_geo_within_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
as_status add_range_predicate_to_query(as_query* query, char* bin_name, long index_type, zval* val, as_error* err);
as_status add_between_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
as_status add_contains_predicate_to_query(as_query* query, char* bin_name, long index_type, zval* val, as_error* err);

as_status add_list_range_predicate_to_query(as_query* query, char* bin_name, long min, long max, as_error* err);
as_status add_mapkeys_range_predicate_to_query(as_query* query, char* bin_name, long min, long max, as_error* err);
as_status add_mapvalues_range_predicate_to_query(as_query* query, char* bin_name, long min, long max, as_error* err);

as_status add_list_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
as_status add_mapkeys_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
as_status add_mapvalues_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err);
// Helper for extracting min max from two element array
as_status get_min_max_from_zval(zval* val, long* min_long, long* max_long, as_error* err);


/* {{{ proto int Aerospike::query( string ns, string set, array where, callback record_cb [, array select [, array options ]] )
    Queries a secondary index on a set for records matching the where predicate  */
PHP_METHOD(Aerospike, query) {

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;
	as_error_init(&err);
	zend_fcall_info callback_info;
	zend_fcall_info_cache callback_cache;
	char* set = NULL;
	char* ns = NULL;
	size_t set_len = 0;
	size_t ns_len = 0;
	HashTable* predicate_array = NULL;
	HashTable* select_bins = NULL;
	uint32_t select_count = 0;
	user_callback_function callback_function_data;

	zval* z_policy = NULL;
	as_policy_query query_policy;
	as_policy_query* query_policy_p = NULL;
	bool query_initialized = false;
	as_query query;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sshf|h!z",
			&ns, &ns_len, &set, &set_len, &predicate_array,
			&callback_info, &callback_cache,
			&select_bins, &z_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to query", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	if (!ns_len) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid namespace", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (z_policy && Z_TYPE_P(z_policy) != IS_NULL) {
		if (zval_to_as_policy_query(z_policy, &query_policy,
				&query_policy_p, &as_client->config.policies.query) != AEROSPIKE_OK) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid query policy", false);
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
		}
		query_policy_p = &query_policy;
	}

	if (!as_query_init(&query, ns, set)) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Unable to create query");
		goto CLEANUP;
	}
	query_initialized = true;

	if (z_policy && Z_TYPE_P(z_policy) != IS_NULL) {
		if (set_query_options_from_policy_hash(&query, z_policy) != AEROSPIKE_OK) {
			RETURN_LONG(AEROSPIKE_ERR_PARAM)
		}
	}

	callback_function_data.callback = callback_info;
	callback_function_data.callback_cache = callback_cache;
	callback_function_data.err = &err;
	callback_function_data.cb_mutex = &AEROSPIKE_G(query_cb_mutex);

	if (select_bins) {
		select_count = zend_hash_num_elements(select_bins);
		if (select_count > 0) {
			as_query_select_inita(&query, select_count);


			zval* entry = NULL;

			ZEND_HASH_FOREACH_VAL(select_bins, entry)
			{
				if (Z_TYPE_P(entry) != IS_STRING) {
					as_error_update(&err, AEROSPIKE_ERR_PARAM, "Bin names must be strings");
					goto CLEANUP;
				}
				as_query_select(&query, Z_STRVAL_P(entry));
			} ZEND_HASH_FOREACH_END();
		}
	}

	if (zend_hash_num_elements(predicate_array)) {
		if (add_predicate_to_query(&query, predicate_array, &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	aerospike_query_foreach(as_client, &err, query_policy_p, &query, user_callback_wrapper, (void*)&callback_function_data);

CLEANUP:
	if (query_initialized) {
		as_query_destroy(&query);
	}

	if ((err.code) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}


	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto in Aerospike::queryApply( string ns, string set, array where,
 * string module, string function, array args, int &job_id [, array options ] )
 * Applies a record UDF to each record of a set using a background query */
PHP_METHOD(Aerospike, queryApply) {
	// TODO: This really takes a write policy at the c client level?
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;
	as_error_init(&err);

	char* set = NULL;
	char* ns = NULL;
	char* module = NULL;
	char* function = NULL;
	size_t set_len = 0;
	size_t ns_len = 0;
	size_t module_len = 0;
	size_t function_len = 0;

	HashTable* z_udf_args = NULL;
	HashTable* predicate_array = NULL;

	as_list* as_args_list = NULL;

	uint64_t job_id = 0;
	zval* z_job_id = NULL;
	zval* z_policy = NULL;
	as_policy_write write_policy;
	as_policy_write* write_policy_p = NULL;

	bool query_initialized = false;

	int serializer_type = INI_INT("aerospike.serializer");
	as_query query;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	//7th by ref
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sshsshz/|z",
			&ns, &ns_len, &set, &set_len,
			&predicate_array, &module, &module_len,
			&function, &function_len, &z_udf_args,
			&z_job_id, &z_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to queryApply", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	zval_dtor(z_job_id);
	ZVAL_NULL(z_job_id);

	if (!ns_len) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid namespace", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_write(z_policy, &write_policy,
			&write_policy_p, &as_client->config.policies.write) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	write_policy_p = &write_policy;

	if (!as_query_init(&query, ns, set)) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Unable to create query");
		goto CLEANUP;
	}
	query_initialized = true;

	if (zend_hash_num_elements(predicate_array)) {
		if (add_predicate_to_query(&query, predicate_array, &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	if (z_hashtable_to_as_list(z_udf_args, &as_args_list, &err, serializer_type) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (!as_query_apply(&query, module, function, as_args_list)) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Failed to initialize arguments");
		as_list_destroy(as_args_list);
		goto CLEANUP;
	}

	if (aerospike_query_background(as_client, &err, write_policy_p, &query, &job_id) == AEROSPIKE_OK) {
		ZVAL_LONG(z_job_id, job_id);
	}

CLEANUP:

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	// Since the query owns the args lists, it will destroy them
	if (query_initialized) {
		as_query_destroy(&query);
	}
	RETURN_LONG(err.code);
}
/* }}} */


typedef struct _aggregate_cb_data {
	as_error* err;
	zval* aggregate_ary;
} aggregate_cb_data ;


bool aggregate_callback_wrapper(const as_val *val, void *udata) {
	zval retval;
	ZVAL_NULL(&retval);
	aggregate_cb_data* cb_data = (aggregate_cb_data*)udata;
	zval* aggregate_list = cb_data->aggregate_ary;

	if (!val) {
		return false;
	}

	as_val_to_zval(val, &retval, cb_data->err);
	// The conversion failed, bail out
	if (cb_data->err->code != AEROSPIKE_OK) {
		return false;
	}

	add_next_index_zval(aggregate_list, &retval);
	return true;
}


/* {{{ proto int Aerospike::aggregate( string ns, string set, array where, string module, string function, array args, mixed &returned [, array options ] )
    Applies a stream UDF to the records matching a query and aggregates the results  */
PHP_METHOD(Aerospike, aggregate) {
	as_error err;
	as_error_init(&err);

	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	char* set = NULL;
	char* ns = NULL;
	char* module = NULL;
	char* function = NULL;
	size_t set_len = 0;
	size_t ns_len = 0;
	size_t module_len = 0;
	size_t function_len = 0;

	HashTable* z_udf_args = NULL;
	HashTable* predicate_array = NULL;

	as_list* as_args_list = NULL;

	zval* z_aggregate_result = NULL;
	zval* z_policy = NULL;
	as_policy_query query_policy;
	as_policy_query* query_policy_p = NULL;

	bool query_initialized = false;

	int serializer_type = INI_INT("aerospike.serializer");
	as_query query;
	aggregate_cb_data cb_data;

	reset_client_error(getThis());

#ifdef ZTS
	update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "aggregate is not supported in ZTS environments", false);
	RETURN_LONG(AEROSPIKE_ERR_CLIENT);
#endif

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sshsshz/|z",
			&ns, &ns_len, &set, &set_len,
			&predicate_array,
			&module, &module_len,
			&function, &function_len,
			&z_udf_args, &z_aggregate_result,
			&z_policy) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters to aggregate", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	// Remove old value of the result reference
	zval_dtor(z_aggregate_result);
	ZVAL_NULL(z_aggregate_result);

	if (!ns_len) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid namespace", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (!module_len) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid module name", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (!function_len) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid function name", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (z_policy && Z_TYPE_P(z_policy) != IS_NULL) {
		if (zval_to_as_policy_query(z_policy, &query_policy,
				&query_policy_p, &as_client->config.policies.query) != AEROSPIKE_OK) {
			update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid query policy", false);
			RETURN_LONG(AEROSPIKE_ERR_PARAM);
		}
		query_policy_p = &query_policy;
	}

	if (!as_query_init(&query, ns, set)) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Unable to create query");
		goto CLEANUP;
	}
	query_initialized = true;

	if (zend_hash_num_elements(predicate_array)) {
		if (add_predicate_to_query(&query, predicate_array, &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	z_hashtable_to_as_list(z_udf_args, &as_args_list, &err, serializer_type);
	if (err.code != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	if (!as_query_apply(&query, module, function, as_args_list)){
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Unable to initialize Argument list");
		as_list_destroy(as_args_list);
		goto CLEANUP;
	}

	cb_data.aggregate_ary = z_aggregate_result;
	cb_data.err = &err;
	array_init(z_aggregate_result);

	if (aerospike_query_foreach(
			as_client, &err, query_policy_p, &query, aggregate_callback_wrapper, (void*)&cb_data) != AEROSPIKE_OK) {
	}


CLEANUP:
	if (query_initialized) {
		as_query_destroy(&query);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);
}
/* }}} */


/* Helper which parses the php user land predicate, and calls another function to actually add it to the query,
 * returns AEROSPIKE_OK on success, error code on failure
 */
as_status add_predicate_to_query(as_query* query, const HashTable* z_predicate_val, as_error* err) {
	zval* bin_val = NULL;
	char* bin_name = NULL;
	zval* val_value = NULL;
	zval* op_val = NULL;
	char* op_str = NULL;

	if (zend_hash_num_elements(z_predicate_val) < 3) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid predicate length");
		return err->code;
	}

	bin_val = zend_hash_str_find(z_predicate_val, QUERY_WHERE_BIN_KEY, strlen(QUERY_WHERE_BIN_KEY));
	if (!bin_val) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Predicate missing bin entry");
		return err->code;
	}
	if (Z_TYPE_P(bin_val) != IS_STRING) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Bin name must be a string");
		return err->code;
	}
	bin_name = Z_STRVAL_P(bin_val);

	op_val = zend_hash_str_find(z_predicate_val, QUERY_WHERE_OP_KEY, strlen(QUERY_WHERE_OP_KEY));
	if (!op_val) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Predicate missing op entry");
		return err->code;
	}
	if (Z_TYPE_P(op_val) != IS_STRING) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid op name");
		return err->code;
	}
	op_str = Z_STRVAL_P(op_val);

	val_value = zend_hash_str_find(z_predicate_val, QUERY_WHERE_VAL_KEY, strlen(QUERY_WHERE_VAL_KEY));
	if (!val_value) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Predicate missing val entry");
		return err->code;
	}

	// Setup the query to accept a where;
	as_query_where_init(query, 1);
	// Array had all required keys, delegate based on op type

	if (!strcmp(op_str, PHP_PREDICATE_EQUAL)) {
		return add_equal_predicate_to_query(query, bin_name, val_value, err);
	}
	if (!strcmp(op_str, PHP_PREDICATE_BETWEEN)) {
		return add_between_predicate_to_query(query, bin_name, val_value, err);
	}
	if (!strcmp(op_str, PHP_PREDICATE_CONTAINS)) {
		zval* index_val = NULL;
		index_val = zend_hash_str_find(z_predicate_val, QUERY_WHERE_INDEX_TYPE_KEY, strlen(QUERY_WHERE_INDEX_TYPE_KEY));
		if (!index_val) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Contains predicate requires index_type");
			return err->code;
		}
		if (Z_TYPE_P(index_val) != IS_LONG) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid index_type");
			return err->code;
		}
		return add_contains_predicate_to_query(query, bin_name, Z_LVAL_P(index_val), val_value, err);
	}
	if (!strcmp(op_str, PHP_PREDICATE_RANGE)) {
		zval* index_val = NULL;
		index_val = zend_hash_str_find(z_predicate_val, QUERY_WHERE_INDEX_TYPE_KEY, strlen(QUERY_WHERE_INDEX_TYPE_KEY));
		if (!index_val) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Contains predicate requires index_type");
			return err->code;
		}
		if (Z_TYPE_P(index_val) != IS_LONG) {
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid index_type");
			return err->code;
		}
		return add_range_predicate_to_query(query, bin_name, Z_LVAL_P(index_val), val_value, err);
	}
	if (!strcmp(op_str, PHP_PREDICATE_GEO_CONTAINS)) {
		return add_geo_contains_predicate_to_query(query, bin_name, val_value, err);
	}
	if (!strcmp(op_str, PHP_PREDICATE_GEO_WITHIN)) {
		return add_geo_within_predicate_to_query(query, bin_name, val_value, err);
	}

	as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid predicate op");
	return err->code;

}

as_status add_equal_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {
	if (Z_TYPE_P(val) == IS_LONG) {
		if (!as_query_where(query, bin_name, as_integer_equals(Z_LVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else if (Z_TYPE_P(val) == IS_STRING) {
		if (!as_query_where(query, bin_name, as_string_equals(Z_STRVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid value for equality predicate");
	}

	return AEROSPIKE_OK;
}


as_status add_between_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {
	long min_long;
	long max_long;

	get_min_max_from_zval(val, &min_long, &max_long, err);
	if (err->code != AEROSPIKE_OK) {
		return err->code;
	}

	if (!as_query_where(query, bin_name, as_integer_range(min_long, max_long))) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Unable to add predicate");
		return err->code;
	}

	return AEROSPIKE_OK;
}

/* Contains predicate helpers */

as_status add_contains_predicate_to_query(as_query* query, char* bin_name, long index_type, zval* val, as_error* err) {
	switch(index_type) {
		case AS_INDEX_TYPE_LIST:
			return add_list_contains_predicate_to_query(query, bin_name, val, err);
		case AS_INDEX_TYPE_MAPKEYS:
			return add_mapkeys_contains_predicate_to_query(query, bin_name, val, err);
		case AS_INDEX_TYPE_MAPVALUES:
			return add_mapvalues_contains_predicate_to_query(query, bin_name, val, err);
		default:
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid Index type");
			return err->code;
	}
}


as_status add_geo_within_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {
	if (Z_TYPE_P(val) != IS_STRING) {
		return as_error_update(err, AEROSPIKE_ERR_CLIENT, "GeoJSON region must be a string");
	}
	if (!as_query_where(query, bin_name, as_geo_within(Z_STRVAL_P(val)))) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
	}
	return err->code;
}


as_status add_geo_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {
	/* TODO HANDLE ARRAY HERE */
	if (Z_TYPE_P(val) != IS_STRING) {
		return as_error_update(err, AEROSPIKE_ERR_CLIENT, "GeoJSON point must be a string");
	}
	if (!as_query_where(query, bin_name, as_geo_contains(Z_STRVAL_P(val)))) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
	}
	return err->code;
}


as_status add_list_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {

	if (Z_TYPE_P(val) == IS_LONG) {
		if (!as_query_where(query, bin_name, as_contains(LIST, NUMERIC, Z_LVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else if (Z_TYPE_P(val) == IS_STRING) {
		if (!as_query_where(query, bin_name, as_contains(LIST, STRING, Z_STRVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid value for equality predicate");
	}
	return AEROSPIKE_OK;
}


as_status add_mapkeys_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {

	if (Z_TYPE_P(val) == IS_LONG) {
		if (!as_query_where(query, bin_name, as_contains(MAPKEYS, NUMERIC, Z_LVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else if (Z_TYPE_P(val) == IS_STRING) {
		if (!as_query_where(query, bin_name, as_contains(MAPKEYS, STRING, Z_STRVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid value for equality predicate");
	}
	return AEROSPIKE_OK;
}


as_status add_mapvalues_contains_predicate_to_query(as_query* query, char* bin_name, zval* val, as_error* err) {

	if (Z_TYPE_P(val) == IS_LONG) {
		if (!as_query_where(query, bin_name, as_contains(MAPVALUES, NUMERIC, Z_LVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else if (Z_TYPE_P(val) == IS_STRING) {
		if (!as_query_where(query, bin_name, as_contains(MAPVALUES, STRING, Z_STRVAL_P(val)))) {
			as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add predicate");
		}
	} else {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid value for equality predicate");
	}
	return AEROSPIKE_OK;
}


as_status add_range_predicate_to_query(as_query* query, char* bin_name, long index_type, zval* val, as_error* err) {
	long min;
	long max;

	get_min_max_from_zval(val, &min, &max, err);

	if (err->code != AEROSPIKE_OK) {
		return err->code;
	}
	switch(index_type) {
		case AS_INDEX_TYPE_LIST:
			return add_list_range_predicate_to_query(query, bin_name, min, max, err);
		case AS_INDEX_TYPE_MAPKEYS:
			return add_mapkeys_range_predicate_to_query(query, bin_name, min, max, err);
		case AS_INDEX_TYPE_MAPVALUES:
			return add_mapvalues_range_predicate_to_query(query, bin_name, min, max, err);
		default:
			as_error_update(err, AEROSPIKE_ERR_PARAM, "Invalid Index type");
			return err->code;
	}
}


as_status add_list_range_predicate_to_query(as_query* query, char* bin_name,
		long min, long max, as_error* err) {
	if (!as_query_where(query, bin_name, as_range(LIST, NUMERIC, min, max))) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add range predicate");
		return err->code;
	}

	return AEROSPIKE_OK;
}


as_status add_mapkeys_range_predicate_to_query(as_query* query, char* bin_name,
		long min, long max, as_error* err) {

	if (!as_query_where(query, bin_name, as_range(MAPKEYS, NUMERIC, min, max))) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add range predicate");
		return err->code;
	}

	return AEROSPIKE_OK;
}


as_status add_mapvalues_range_predicate_to_query(as_query* query, char* bin_name,
		long min, long max, as_error* err) {

	if (!as_query_where(query, bin_name, as_range(MAPVALUES, NUMERIC, min, max))) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Unable to add range predicate");
		return err->code;
	}

	return AEROSPIKE_OK;
}


/*
 * takes a php array of the form [1,3] and sets to longs to have those values
 * returns error if the php value is of the wrong type, or the array is malformed
 *
 */
as_status get_min_max_from_zval(zval* val, long* min_long, long* max_long, as_error* err) {
	HashTable* min_max_hash = NULL;
	zval* min = NULL;
	zval* max = NULL;

	if (Z_TYPE_P(val) != IS_ARRAY) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "val entry for predicate must be an array");
		return err->code;
	}
	min_max_hash = Z_ARRVAL_P(val);

	if (zend_hash_num_elements(min_max_hash) != 2) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "val entry for predicate must contain exactly 2 entries");
		return err->code;
	}

	if (!hashtable_is_list(min_max_hash)) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "val entry for predicate must be a list");
		return err->code;
	}

	min = zend_hash_index_find(min_max_hash, 0);
	if (!min) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to find min entry");
		return err->code;
	}
	if (Z_TYPE_P(min) != IS_LONG) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "min entry must be an integer");
		return err->code;
	}
	*min_long = Z_LVAL_P(min);

	max = zend_hash_index_find(min_max_hash, 1);
	if (!max) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "Failed to find max entry");
		return err->code;
	}
	if (Z_TYPE_P(max) != IS_LONG) {
		as_error_update(err, AEROSPIKE_ERR_PARAM, "max entry must be an integer");
		return err->code;
	}
	*max_long = Z_LVAL_P(max);

	return AEROSPIKE_OK;
}
