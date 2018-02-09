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
#include "aerospike_class.h"
#include "policy_conversions.h"
#include "php_aerospike_types.h"
#include "conversions.h"
#include "aerospike/aerospike_udf.h"


//aerospike.udf.lua_user_path stores the base path
#define LUA_FILE_BUFFER_FRAME 512 // READ BLOCK SIZE FOR copying the udf into the location
#define SCRIPT_LEN_MAX 1048576
#define COPY_FILEPATH_SIZE AS_CONFIG_PATH_MAX_SIZE * 2
#define COPY_FILEPATH_MAXLEN COPY_FILEPATH_SIZE - 1


/* {{{ proto int Aerospike::getRegistered( string module, string &code [, int language=Aerospike::UDF_TYPE_LUA [, array options ]] )
    Gets the code for a UDF module registered with the cluster */
PHP_METHOD(Aerospike, getRegistered) {
	//Client Vars
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;

	//Policy Vars
	zval* z_info_policy = NULL;
	as_policy_info info_policy;
	as_policy_info *info_policy_p = NULL;

	//Param vars
	zval* returned_code = NULL; // Reference variable to be filled
	char* module = NULL;
	zend_long language = AS_UDF_TYPE_LUA;
	size_t module_len = 0;

	bool file_initialized = false;

	as_error_init(&err);
	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/|lz",
			&module, &module_len, &returned_code, &language, &z_info_policy) != SUCCESS) {
				update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for getRegistered", false);
				RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	// Cleanup any old value held by the reference
	zval_dtor(returned_code);
	ZVAL_NULL(returned_code);

	if (zval_to_as_policy_info(z_info_policy, &info_policy,
			&info_policy_p, &as_client->config.policies.info) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid information policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	info_policy_p = &info_policy;


	as_udf_file file;
	as_udf_file_init(&file);
	file_initialized = true;

	aerospike_udf_get(as_client, &err, info_policy_p, module, language, &file);
	if (err.code != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	char* code_bytes = (char*)file.content.bytes;
	uint32_t code_len = file.content.size;

	ZVAL_STRINGL(returned_code, code_bytes, code_len);

CLEANUP:
	if (file_initialized) {
		as_udf_file_destroy(&file);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}
/* }}} */


/* {{{ proto int Aerospike::register( string path, string module [, int language=Aerospike::UDF_TYPE_LUA [, array options ]] )
    Registers a UDF module with the cluster  */
PHP_METHOD(Aerospike, register) {
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;

	char* file_path = NULL;
	char* udf_file_name = NULL;
	char* module = NULL;
	char* user_base_path = NULL;
	char copy_file_path[COPY_FILEPATH_SIZE] = {0};

	size_t path_len, module_len;

	zend_long udf_language = AS_UDF_TYPE_LUA;
	zval* z_info_policy = NULL;
	as_policy_info info_policy;
	as_policy_info *info_policy_p = NULL;
	int user_base_path_len = 0;
	uint32_t file_size = 0;
	FILE* write_file = NULL;
	FILE* read_file = NULL;
	uint8_t* bytes = NULL;
	as_error_init(&err);
	as_bytes contents;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|lz",
			&file_path, &path_len, &module, &module_len, &udf_language, &z_info_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid Parameters for register", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (module_len == 0) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Empty filename for udf", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}


	if (zval_to_as_policy_info(z_info_policy, &info_policy, &info_policy_p,
			&as_client->config.policies.info) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid information policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	info_policy_p = &info_policy;


	/* Open and validate the inFile*/
	read_file = fopen(file_path, "r");

	if (!read_file) {
		as_error_update(&err, AEROSPIKE_ERR_UDF_NOT_FOUND, "Unable to open UDF file");
		err.code = AEROSPIKE_ERR_UDF_NOT_FOUND;
		goto CLEANUP;
	}

	fseek(read_file, 0L, SEEK_END);
	file_size = ftell(read_file);
	fseek(read_file, 0L, SEEK_SET);

	if (file_size <= 0) {
		as_error_update(&err, AEROSPIKE_ERR_UDF, "UDF file is empty");
		err.code = AEROSPIKE_ERR_UDF;
		goto CLEANUP;
	}

	if (file_size >= SCRIPT_LEN_MAX) {
		as_error_update(&err, AEROSPIKE_ERR_UDF, "UDF file is too large");
		err.code = AEROSPIKE_ERR_UDF;
		goto CLEANUP;
	}

	/* Construct the copy path for the local version of the item to be written */
	user_base_path = INI_STR("aerospike.udf.lua_user_path");
	if (!user_base_path) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Base path not found");
		err.code = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;
	}
	user_base_path_len = strlen(user_base_path);

	memcpy(copy_file_path, user_base_path, user_base_path_len);

	// Add a trailing slash to the user udf path if it doesn't include one
	if (copy_file_path[user_base_path_len - 1] != '/') {
		memset(copy_file_path + user_base_path_len, '/', 1);
		user_base_path_len += 1;
	}

	udf_file_name = strrchr(file_path, '/');
	if (!udf_file_name) {
		udf_file_name = file_path;
	} else {
		// If the file ended in a / this is an error
		if (udf_file_name - file_path == path_len - 1) {
			as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Base path not found");
			err.code = AEROSPIKE_ERR_CLIENT;
			goto CLEANUP;
		}
		udf_file_name++; //advance to the character after the rightmost /
	}
	//
	//if (user_base_path_len + )
	path_len = strlen(udf_file_name);
	if (user_base_path_len + path_len > COPY_FILEPATH_MAXLEN) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Path name too long");
		err.code = AEROSPIKE_ERR_PARAM;
		goto CLEANUP;
	}

	memcpy(copy_file_path + user_base_path_len, udf_file_name, path_len);

	/* Open and validate the outFile*/
	write_file = fopen(copy_file_path, "w");

	if (!write_file) {
		as_error_update(&err, AEROSPIKE_ERR_LUA_FILE_NOT_FOUND, "Unable to open the destination UDF file");
		err.code = AEROSPIKE_ERR_LUA_FILE_NOT_FOUND;
		goto CLEANUP;
	}


	bytes = (uint8_t*)malloc(SCRIPT_LEN_MAX);
	if (!bytes) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Failed to allocate buffer for UDF file");
		err.code = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;
	}

	uint8_t* buff = bytes;
	int size = 0;
	int read = (int)fread(buff, 1, LUA_FILE_BUFFER_FRAME, read_file);
	if (read && fwrite(buff, 1, read, write_file)) {
		while(read) {
			size += read;
			buff += read;
			read = (int)fread(buff, 1, LUA_FILE_BUFFER_FRAME, read_file);
			if (!fwrite(buff, 1, read, write_file) ){
				break;
			}
		}
	} else {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Unable to write to user path");
		err.code = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;
	}

	as_bytes_init_wrap(&contents, bytes, size, true);

	aerospike_udf_put(as_client, &err, info_policy_p, module, udf_language, &contents);

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		goto CLEANUP;
	}

	aerospike_udf_put_wait(as_client, &err, info_policy_p, module, 2000);
	// setup of files is complete, begin reading

CLEANUP:
	if (write_file) {
		fclose(write_file);
	}
	if (read_file) {
		fclose(read_file);
	}
	if (bytes) {
		free(bytes);
	}
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::deregister( string module [, array options ] )
    Removes a UDF module from the cluster  */
PHP_METHOD(Aerospike, deregister) {
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;

	char* module = NULL;
	size_t module_len;

	zval* z_info_policy = NULL;
	as_policy_info info_policy;
	as_policy_info *info_policy_p = NULL;

	as_error_init(&err);
	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, false);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|z",
			&module, &module_len, &z_info_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters for deregister", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	if (zval_to_as_policy_info(z_info_policy, &info_policy,
			&info_policy_p, &as_client->config.policies.info) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid information policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	info_policy_p = &info_policy;


	aerospike_udf_remove(as_client, &err, info_policy_p, module);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	RETURN_LONG(err.code);

}
/* }}} */


/* {{{ proto int Aerospike::listRegistered( array &modules [, int language [, array options ]] )
    Lists the UDF modules registered with the cluster */
PHP_METHOD(Aerospike, listRegistered) {
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;
	as_error err;
	zval current_file;
	as_udf_files files;

	zval* modules = NULL;
	zend_long language = AS_UDF_TYPE_LUA;
	zval* z_info_policy = NULL;
	as_policy_info info_policy;
	as_policy_info *info_policy_p = NULL;

	bool udf_files_initialized = false;
	as_error_init(&err);
	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		RETURN_LONG(err.code);
	}

	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z/|lz",
			&modules, &language, &z_info_policy) != SUCCESS) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid parameters for deRegister", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

	zval_dtor(modules);
	ZVAL_NULL(modules);

	if (zval_to_as_policy_info(z_info_policy, &info_policy,
			&info_policy_p, &as_client->config.policies.info) != AEROSPIKE_OK) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid information policy", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	info_policy_p = &info_policy;

	as_udf_files_init(&files, 0);
	udf_files_initialized = true;

	aerospike_udf_list(as_client, &err, info_policy_p, &files);
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		goto CLEANUP;
	}

	array_init(modules);
    for( int i = 0; i < files.size; i++ ) {
        as_udf_file file = files.entries[i];
        if (file.type == language) {
            array_init(&current_file);
            add_assoc_string(&current_file, "name", file.name);
            add_assoc_long(&current_file, "type", file.type);
            add_next_index_zval(modules, &current_file);
        }
    }

CLEANUP:
	if(udf_files_initialized) {
		as_udf_files_destroy(&files);
	}
	RETURN_LONG(err.code);
}
/* }}} */
