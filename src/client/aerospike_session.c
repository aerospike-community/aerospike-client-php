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
#include "php_aerospike_types.h"
#include "aerospike_session.h"
#include "aerospike/aerospike_key.h"
#include "persistent_list.h"
#include "aerospike/as_config.h"

#define AEROSPIKE_SESSION_BIN "PHP_SESSION"
#define NS_SET_DELIM '|'

typedef struct aerospike_session_data_ {
	AerospikeClient* php_client;
	as_namespace ns;
	as_set set;
} aerospike_session_data;

ps_module ps_mod_aerospike = {
	PS_MOD_UPDATE_TIMESTAMP(aerospike)
};

#define PS_AEROSPIKE_DATA aerospike_session_data *data = (aerospike_session_data*)PS_GET_MOD_DATA()

static bool set_up_session_data_from_save_path(aerospike_session_data* session_data, const char* save_path);
static void free_aerospike_session(aerospike_session_data* session);
static bool verify_session_data(aerospike_session_data* session_data);
static bool aerospike_session_exists(aerospike_session_data* session_data, const char* exists_sid);

/* Testing function */
void call_setup_function(const char* str) {

	aerospike_session_data* aerospike_session;
	aerospike_session = malloc(sizeof(aerospike_session_data));
	aerospike_session->php_client = NULL;
	set_up_session_data_from_save_path(aerospike_session, str);
}

PS_CLOSE_FUNC(aerospike) {
	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	/* If the session isn't valid, we don't free resources, but this call should never fail */
	if (verify_session_data(session)) {
		free_aerospike_session(session);
	}
	/* This is a shared connection so we don't want to actually close the database connection */

	PS_SET_MOD_DATA(NULL);
	return SUCCESS;
}

PS_WRITE_FUNC(aerospike) {
	as_status status = AEROSPIKE_OK;
	bool key_init = false;
	bool record_init = false;
	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	as_key write_key;
	as_error err;
	as_error_init(&err);
	as_record record;

	if (!verify_session_data(session)) {
		return FAILURE;
	}

	if (!as_key_init_str(&write_key, session->ns, session->set, ZSTR_VAL(key))) {
		return FAILURE;
	}
	key_init = true;

	as_record_inita(&record, 1);
	record_init = true;

	if (!as_record_set_raw_typep(&record, AEROSPIKE_SESSION_BIN, (uint8_t*)ZSTR_VAL(val),
			ZSTR_LEN(val), AS_BYTES_PHP, false)) {
		status = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;

	}

	record.ttl = (uint32_t)maxlifetime;

	status = aerospike_key_put(session->php_client->as_client, &err, NULL, &write_key, &record);

CLEANUP:
	if (key_init) {
		as_key_destroy(&write_key);
	}
	if (record_init) {
		as_record_destroy(&record);
	}
	return (status == AEROSPIKE_OK) ? SUCCESS : FAILURE;
}

PS_DESTROY_FUNC(aerospike) {

	as_status status = AEROSPIKE_OK;
	bool key_init = false;

	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();

	if (!verify_session_data(session)) {
		return FAILURE;
	}

	as_key remove_key;
	as_error err;
	as_error_init(&err);

	if (!as_key_init_str(&remove_key, session->ns, session->set, ZSTR_VAL(key))) {
		return FAILURE;
	}
	key_init = true;

	status = aerospike_key_remove(session->php_client->as_client, &err, NULL, &remove_key);

	if (key_init) {
		as_key_destroy(&remove_key);
	}
	/* It's fine if the current ID had no data in it, so return success for that case */
	return (status == AEROSPIKE_OK || status == AEROSPIKE_ERR_RECORD_NOT_FOUND) ? SUCCESS : FAILURE;
}

PS_GC_FUNC(aerospike) {

	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	if (!verify_session_data(session)) {
		return FAILURE;
	}
	return SUCCESS;
}

PS_READ_FUNC(aerospike) {
	as_status status = AEROSPIKE_OK;
	bool key_init = false;
	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	as_key read_key;
	as_error err;
	as_error_init(&err);
	as_record* record = NULL;
	as_bytes* bytes = NULL;
	as_string* string = NULL;

	*val = NULL;

	if (!verify_session_data(session)) {
		php_error_docref(NULL, E_WARNING, "Invalid session");
		return FAILURE;
	}

	if (!as_key_init_str(&read_key, session->ns, session->set, ZSTR_VAL(key))) {
		php_error_docref(NULL, E_WARNING, "Failed to create an aerospike key");
		return FAILURE;
	}
	key_init = true;

	status = aerospike_key_get(session->php_client->as_client, &err, NULL, &read_key, &record);
	/* It's ok if there is not yet an entry for this ID, or it's been evicted. Return an empty string
	 * In this case.
	 */
	if (status == AEROSPIKE_ERR_RECORD_NOT_FOUND) {
		*val = ZSTR_EMPTY_ALLOC();
		status = AEROSPIKE_OK;
		goto CLEANUP;
	}
	if (status != AEROSPIKE_OK) {
		php_error_docref(NULL, E_WARNING, "Failed to fetch session entry: %s", err.message);
		goto CLEANUP;
	}

	as_val* session_val  = (as_val*)as_record_get(record, AEROSPIKE_SESSION_BIN);
	if (!session_val) {
		php_error_docref(NULL, E_WARNING, "No session data found in record");
		status = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;
	}
	switch(as_val_type(session_val)) {

		case AS_BYTES:
			bytes = as_bytes_fromval(session_val);
			*val = zend_string_init((char*)as_bytes_get(bytes), as_bytes_size(bytes), 0);
			break;

		case AS_STRING:
			string = as_string_fromval(session_val);
			*val = zend_string_init((char*)as_string_get(string), as_string_len(string), 0);
			break;
		default:
			/* This should always be a string or bytes */
			php_error_docref(NULL, E_WARNING, "Invalid session data returned");
			status = AEROSPIKE_ERR_CLIENT;
	}


CLEANUP:
	if (key_init) {
		as_key_destroy(&read_key);
	}
	if (record) {
		as_record_destroy(record);
	}
	return (status == AEROSPIKE_OK) ? SUCCESS : FAILURE;
}

PS_OPEN_FUNC(aerospike) {

	bool valid = true;
	aerospike_session_data* aerospike_session;
	aerospike_session = malloc(sizeof(aerospike_session_data));
	aerospike_session->php_client = NULL;

	valid = set_up_session_data_from_save_path(aerospike_session, save_path);

	if (!valid) {
		free(aerospike_session);
		return FAILURE;
	}
	if (PS_GET_MOD_DATA()) {
		free_aerospike_session((aerospike_session_data*)PS_GET_MOD_DATA());
	}
	PS_SET_MOD_DATA(aerospike_session);
	return SUCCESS;
}

PS_CREATE_SID_FUNC(aerospike) {
	int max_attempts = 5;
	zend_string* sid = NULL;
	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	if (!verify_session_data(session)) {
		return NULL;
	}

	for(; max_attempts > 0; max_attempts--) {
		sid = php_session_create_id(mod_data);
		if (!sid) {
			continue;
		}
		if (aerospike_session_exists(session, ZSTR_VAL(sid))) {
			zend_string_release(sid);
			sid = NULL;
			continue;
		}
		return sid;
	}
	return NULL;
}

PS_VALIDATE_SID_FUNC(aerospike) {
	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	if (!verify_session_data(session)) {
		return FAILURE;
	}
	return (aerospike_session_exists(session, ZSTR_VAL(key))) ? SUCCESS : FAILURE;

}

PS_UPDATE_TIMESTAMP_FUNC(aerospike) {
	aerospike_session_data* session = (aerospike_session_data*)PS_GET_MOD_DATA();
	if (!verify_session_data(session)) {
		php_error_docref(NULL, E_WARNING, "Invalid session in update timestamp function");
		return FAILURE;
	}
	as_key touch_key;
	as_error err;
	as_error_init(&err);
	as_operations ops;
	as_record* rec = NULL;
	if (!as_key_init(&touch_key, session->ns, session->set, ZSTR_VAL(key))) {
		return FAILURE;
	}

	as_operations_inita(&ops, 1);
	as_operations_add_touch(&ops);
	ops.ttl = (uint32_t)maxlifetime;

	aerospike_key_operate(session->php_client->as_client, &err, NULL, &touch_key, &ops, &rec);

	if (rec) {
		as_record_destroy(rec);
	}
	as_key_destroy(&touch_key);
	as_operations_destroy(&ops);

	return (err.code == AEROSPIKE_OK) ? SUCCESS : FAILURE;
}
/*
 * session_data must not be null at this point
 *
 */
static bool set_up_session_data_from_save_path(aerospike_session_data* session_data, const char* save_path) {
	as_config config;
	as_config_init(&config);
	bool success = true;
	char* save_copy = strdup(save_path);

	int length;

	char* start = save_copy;

	char* delim_pos = strchr(start, NS_SET_DELIM);
	if (!delim_pos) {
		php_error_docref(NULL, E_WARNING, "Namespace not found in session path");
		success = false;
		goto CLEANUP;
	}
	length = delim_pos - start;

	if (!length || length >= AS_NAMESPACE_MAX_SIZE) {
		php_error_docref(NULL, E_WARNING, "Namespace is too long");
		success = false;
		goto CLEANUP;
	}

	strncpy(session_data->ns, start, length + 1);
	session_data->ns[length] = '\0';

	/* if the path ended with | bail out */
	start = delim_pos + 1;
	if (*start == '\0') {
		php_error_docref(NULL, E_WARNING, "Set not found in session path");
		success = false;
		goto CLEANUP;
	}

	delim_pos = strchr(start, NS_SET_DELIM);
	if (!delim_pos) {
		php_error_docref(NULL, E_WARNING, "Invalid session path");
		success = false;
		goto CLEANUP;
	}

	length = delim_pos - start;
	if (!length || length >= AS_SET_MAX_SIZE) {
		php_error_docref(NULL, E_WARNING, "Set name is too long");
		success = false;
		goto CLEANUP;
	}
	strncpy(session_data->set, start, length + 1);
	session_data->set[length] = '\0';

	start = delim_pos + 1;
	if (*start == '\0') {
		php_error_docref(NULL, E_WARNING, "host information not found in session path");
		success = false;
		goto CLEANUP;
	}

	if (!as_config_add_hosts(&config, start, 3000)) {
		php_error_docref(NULL, E_WARNING, "Invalid hosts entry");
		success = false;
		goto CLEANUP;
	}

/* ZTS Runs into problems with aerospike c client callbacks, so disable them */
#ifdef ZTS
	config.thread_pool_size = 0;
#endif

	session_data->php_client = (AerospikeClient*)malloc(sizeof(AerospikeClient));
	session_data->php_client->is_connected = false;
	session_data->php_client->is_persistent = true;
	set_policy_defaults_from_ini(&config, session_data->php_client);
	session_data->php_client->as_client = aerospike_new(&config);
	if (!session_data->php_client->as_client) {
		php_error_docref(NULL, E_WARNING, "Failed to create session");
		free(session_data->php_client);
		session_data->php_client = NULL;
		success = false;
		goto CLEANUP;
	}
	session_data->php_client->is_valid = true;


	if (persistent_connect(session_data->php_client) != AEROSPIKE_OK) {
		/* Since we failed to connect here, it's safe to destroy this */
		php_error_docref(NULL, E_WARNING, "Failed to connect session to Aerospike server");
		aerospike_destroy(session_data->php_client->as_client);
		free(session_data->php_client);
		session_data->php_client = NULL;
		success = false;
		goto CLEANUP;
	}

	session_data->php_client->is_connected = true;

CLEANUP:
	free(save_copy);
	return success;
}

/* Frees any memory allocated for the session so far */
static void free_aerospike_session(aerospike_session_data* session) {
	/* This should always be true */
	if (session && session->php_client) {
		free(session->php_client);
		session->php_client = NULL;
		free(session);
	}
}

static bool verify_session_data(aerospike_session_data* session_data) {
	return (session_data && session_data->php_client && session_data->php_client->as_client);
}


 /*
 ** This will return true if there exists a record in the database for the provided sid,
 ** false if it does not exist or fails.
 */
static bool aerospike_session_exists(aerospike_session_data* session_data, const char* exists_sid) {
	as_key exists_key;
	as_error err;
	as_error_init(&err);

	as_record* rec = NULL;
	if (!verify_session_data(session_data)) {
		php_error_docref(NULL, E_WARNING, "Invalid Session data in exists");
		return false;
	}

	if (!as_key_init(&exists_key, session_data->ns, session_data->set, exists_sid)) {
		php_error_docref(NULL, E_WARNING, "Failed to create key for session existence");
		return false;
	}
	aerospike_key_exists(session_data->php_client->as_client, &err, NULL, &exists_key, &rec);

	if (rec) {
		as_record_destroy(rec);
	}
	as_key_destroy(&exists_key);

	if ( (err.code != AEROSPIKE_OK) && (err.code != AEROSPIKE_ERR_RECORD_NOT_FOUND) ) {
		php_error_docref(NULL, E_WARNING, "Existence check failed: %s", err.message);
		return false;
	}

	return (err.code == AEROSPIKE_OK);
}
