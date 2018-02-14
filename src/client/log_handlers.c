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

/*
 *******************************************************************************************************
 * Callback for C client's logger.
 * This function shall be invoked by:
 * 1. C client's logger statements.
 *
 * @param level             The as_log_level to be used by the callback.
 * @param func              The function name generating the log.
 * @param file              The file name containing the func generating the log.
 * @param line              The line number in file where the log was generated.
 * @param fmt               The format specifier for logger.
 *
 * @return true if log callback succeeds. Otherwise false.
 *******************************************************************************************************
 */

static bool aerospike_helper_log_callback(as_log_level level, const char * func, const char * file, uint32_t line, const char * fmt, ...)
{

	zend_fcall_info log_call_info;
	zend_fcall_info_cache log_call_info_cache;
	zval function_args[4];
	zval function_name;
	zval file_name;
	zval line_number;
	zval log_level;
	zval retval;
	bool call_status = true;

/* Accessing global thread local storage will cause issues here, so we just return if ZTS is enabled */
#ifdef ZTS
	return true;
#endif

	ZVAL_NULL(&retval);

	/* This should not ever happen, but if it does indicate logging failure */
	if (!AEROSPIKE_G(is_log_callback_registered)) {
		return false;
	}

	memcpy(&log_call_info, &AEROSPIKE_G(log_callback_call_info), sizeof(zend_fcall_info));
	memcpy(&log_call_info_cache, &AEROSPIKE_G(log_callback_call_info_cache), sizeof(zend_fcall_info_cache));

	ZVAL_LONG(&log_level, level);
	ZVAL_LONG(&line_number, line);

	if (func) {
		ZVAL_STRING(&function_name, func);
	} else {
		ZVAL_STRING(&function_name, "");
	}

	if (file) {
		ZVAL_STRING(&file_name, file);
	} else {
		ZVAL_STRING(&file_name, "");
	}

	log_call_info.param_count = 4;
	log_call_info.retval = &retval;

	function_args[0] = log_level;
	function_args[1] = function_name;
	function_args[2] = file_name;
	function_args[3] = line_number;

	log_call_info.params = function_args;

	if (zend_call_function(&log_call_info, &log_call_info_cache) == FAILURE) {
		call_status = false;
	}

	zval_dtor(&file_name);
	zval_dtor(&function_name);

	zval_dtor(&retval);
	return call_status;
}


/* API Visible methods */

/* {{{
 *
 * proto int Aerospike::setLogLevel(int log_level);
 * Set the logging threshhold of the Aerospike client
 * Note that this is a global value
 */
PHP_METHOD(Aerospike, setLogLevel) {
	zend_long log_level;
	reset_client_error(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &log_level) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid arguments to setLogLevel", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}
	as_log_set_level(log_level);

	RETURN_LONG(AEROSPIKE_OK);

}
/* }}} */


/* {{{ proto Aerospike::setLogHandler( callback log_handler )
   Sets a handler for log events of the Aerospike client
*
*	The callback function should be defined as:
*	bool logger(int log_level, string function_name, string file_name, int line_number)

*	the function should return true when logging succeeded and false otherwise
*
*/
PHP_METHOD(Aerospike, setLogHandler) {
	zend_fcall_info log_handler;
	zend_fcall_info_cache log_handler_cache;

	reset_client_error(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "f",
			&log_handler, &log_handler_cache) == FAILURE) {
		update_client_error(getThis(), AEROSPIKE_ERR_PARAM, "Invalid argument to setLogHandler", false);
		RETURN_LONG(AEROSPIKE_ERR_PARAM);
	}

/* Since non ZE threads can call the callback, TLS will cause issues, so disallow this */
#ifdef ZTS
	update_client_error(getThis(), AEROSPIKE_ERR_CLIENT, "User log callback not currently supported in ZTS environments", false);
	RETURN_LONG(AEROSPIKE_ERR_CLIENT);
#endif

	/* If there was a log callback registered before, we need to release it */
	if AEROSPIKE_G(is_log_callback_registered) {
		zval_dtor(&AEROSPIKE_G(log_callback_call_info).function_name);
		/* This needs to be set to false until the new function is registered */
		AEROSPIKE_G(is_log_callback_registered) = false;
	}

	/* Add a reference since the function name goes out of scope after this function call */
	Z_TRY_ADDREF(log_handler.function_name);
	memcpy(&AEROSPIKE_G(log_callback_call_info), &log_handler, sizeof(zend_fcall_info));

	memcpy(&AEROSPIKE_G(log_callback_call_info_cache), &log_handler_cache, sizeof(zend_fcall_info_cache));
	AEROSPIKE_G(is_log_callback_registered) = true;

	as_log_set_callback((as_log_callback) aerospike_helper_log_callback);

	RETURN_LONG(AEROSPIKE_OK);
}
/* }}} */
