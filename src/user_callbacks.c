#include "user_callbacks.h"
#include "conversions.h"

as_status execute_user_callback(as_record* record, user_callback_function* callback) {

	zval z_record;
	zval z_params[1];

	as_status status  = as_record_to_zval(record, &z_record, NULL, true, callback->err);

	if (status != AEROSPIKE_OK) {
		return callback->err->code;
	}

	z_params[0] = z_record;
	callback->callback.param_count = 1;
	callback->callback.params = z_params;

	// Exclude any other threads from running the callback
	// This technically means that the callback does not need to be thread safe
	if (zend_call_function(&callback->callback, &callback->callback_cache) != SUCCESS) {
		zval_dtor(&z_record);
		callback->err->code = AEROSPIKE_ERR_CLIENT;
		return AEROSPIKE_ERR_CLIENT;
	}
	// clean up the record

	zval_dtor(&z_record);
	return AEROSPIKE_OK;
}

bool user_callback_wrapper(const as_val *val, void *udata) {
	zval retval;
	ZVAL_NULL(&retval);
	if (!val) {
		return false;
	}
	as_record* record = as_record_fromval(val);
	user_callback_function* callback_info = (user_callback_function*)udata;

	pthread_mutex_lock(callback_info->cb_mutex);
	callback_info->callback.retval = &retval;

	if (execute_user_callback(record, callback_info) != AEROSPIKE_OK) {
		as_error_update(callback_info->err, AEROSPIKE_ERR_PARAM, "Callback raised an error");
		pthread_mutex_unlock(callback_info->cb_mutex);
		return false;
	}

	if (callback_info->callback.retval &&
			Z_TYPE_P(callback_info->callback.retval) == IS_FALSE) {
		pthread_mutex_unlock(callback_info->cb_mutex);
		return false;
	}

	zval_dtor(&retval);
	pthread_mutex_unlock(callback_info->cb_mutex);
	return true;
}
