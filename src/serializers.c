#include "serializers.h"
#include "aerospike/as_error.h"
#include "aerospike_class.h"
#include "php_aerospike_types.h"


as_status serialize_with_user_function(const zval* zval_to_serialize, as_bytes** serialized_bytes, as_error* err, int serializer_type) {
	zend_fcall_info local_serializer;
	zend_fcall_info_cache local_serializer_cache;
	zval z_bytes_str;
	ZVAL_UNDEF(&z_bytes_str);
	zval z_params[1];
	size_t bytes_len;

	switch(serializer_type) {
		case SERIALIZER_NONE: {
			as_error_set_message(err, AEROSPIKE_ERR_PARAM, "SERIALIZER_NONE set when serialization is needed");
			return AEROSPIKE_ERR_PARAM;
		}
		case SERIALIZER_PHP: {
			break;
		}
		case SERIALIZER_USER: {

			if (!AEROSPIKE_G(is_global_user_serializer_registered)) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "No serializer registered");
				return AEROSPIKE_ERR_CLIENT;
			}
			memcpy(&local_serializer, &AEROSPIKE_G(user_global_serializer_call_info), sizeof(zend_fcall_info));
			memcpy(&local_serializer_cache, &AEROSPIKE_G(user_global_serializer_call_info_cache), sizeof(zend_fcall_info_cache));

			//params is array of zval so we need to dereference this
			z_params[0] = *zval_to_serialize;
			local_serializer.retval = &z_bytes_str;
			local_serializer.params= z_params;
			local_serializer.param_count = 1;
			if(zend_call_function(&local_serializer, &local_serializer_cache) != SUCCESS) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Serialization failed");
				zval_dtor(&z_bytes_str);
				return AEROSPIKE_ERR_CLIENT; // the user serialization failed.
			}

			if (Z_TYPE(z_bytes_str) != IS_STRING) {
				as_error_update(err, AEROSPIKE_ERR_CLIENT, "Serializer must return a string");
				zval_dtor(&z_bytes_str);
				return AEROSPIKE_ERR_CLIENT; // This should never happen;
			}

			bytes_len = Z_STRLEN(z_bytes_str);
			*serialized_bytes = as_bytes_new(bytes_len);

			as_bytes_set(*serialized_bytes, 0, (const uint8_t*)Z_STRVAL(z_bytes_str), bytes_len);
			as_bytes_set_type(*serialized_bytes, AS_BYTES_BLOB);
			zval_dtor(&z_bytes_str);
			return AEROSPIKE_OK;
		}
		default: break;
	}
	return AEROSPIKE_OK;
}


as_status unserialize_with_user_function(const as_bytes* bytes, zval* return_zval, as_error* err) {

	zend_fcall_info local_deserializer;
	zend_fcall_info_cache local_deserializer_cache;
	memcpy(&local_deserializer, &AEROSPIKE_G(user_global_deserializer_call_info), sizeof(zend_fcall_info));
	memcpy(&local_deserializer_cache, &AEROSPIKE_G(user_global_deserializer_call_info_cache), sizeof(zend_fcall_info_cache));
	zval z_bytes_str;
	zval z_params[1];
	size_t bytes_len;
	bytes_len = bytes->size;

	if (!AEROSPIKE_G(is_global_user_deserializer_registered)) {
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "No deserializer registered");
		return AEROSPIKE_ERR_CLIENT;
	}


	ZVAL_STRINGL(&z_bytes_str, (char*)bytes->value, bytes_len);
	z_params[0] = z_bytes_str;
	local_deserializer.param_count = 1;
	local_deserializer.params = z_params;
	local_deserializer.retval = return_zval;
	if(zend_call_function(&local_deserializer, &local_deserializer_cache) != SUCCESS) {
		zval_dtor(&z_bytes_str);
		as_error_update(err, AEROSPIKE_ERR_CLIENT, "Deserialization failed");
		return AEROSPIKE_ERR_PARAM; // the user serialization failed.
	}
	zval_dtor(&z_bytes_str);
	return AEROSPIKE_OK;

}
