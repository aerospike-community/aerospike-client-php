
#ifndef PHP_AEROSPIKE_H
#define PHP_AEROSPIKE_H

extern zend_module_entry aerospike_module_entry;
#define phpext_aerospike_ptr &aerospike_module_entry


#define PHP_AEROSPIKE_VERSION "7.5.2"

#ifdef PHP_WIN32
#	define PHP_AEROSPIKE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_AEROSPIKE_API __attribute__ ((visibility("default")))
#else
#	define PHP_AEROSPIKE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "aerospike/as_error.h"
#include "pthread.h"



ZEND_BEGIN_MODULE_GLOBALS(aerospike)
	int nesting_depth;
	int connect_timeout;
	int read_timeout;
	int write_timeout;
	char *log_path;
	char *log_level;
	int serializer;
	char *lua_user_path;
	int key_policy;
	int key_gen;
	zend_bool shm_use;
	zend_bool use_batch_direct;
	int max_threads;
	int thread_pool_size;
	int shm_max_nodes;
	int shm_max_namespaces;
	int shm_takeover_threshold_sec;
	int shm_key;
	int shm_key_counter;
	int compression_threshold;
	as_error global_error;
	HashTable *persistent_list_g;
	HashTable *shm_key_list_g;
	int persistent_ref_count;
	int shm_key_ref_count;
	zend_fcall_info user_global_deserializer_call_info;
	zend_fcall_info_cache user_global_deserializer_call_info_cache;
	uint32_t is_global_user_deserializer_registered;
	zend_fcall_info user_global_serializer_call_info;
	zend_fcall_info_cache user_global_serializer_call_info_cache;
	uint32_t is_global_user_serializer_registered;
	uint32_t is_log_callback_registered;
	zend_fcall_info log_callback_call_info;
	zend_fcall_info_cache log_callback_call_info_cache;

	pthread_mutex_t query_cb_mutex;
ZEND_END_MODULE_GLOBALS(aerospike)

ZEND_EXTERN_MODULE_GLOBALS(aerospike);

/* Always refer to the globals in your function as AEROSPIKE_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define AEROSPIKE_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(aerospike, v)

#if defined(ZTS) && defined(COMPILE_DL_AEROSPIKE)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

//#define UPDATE_CURRENT_ERROR(code, msg) as_error_set_message(&AEROSPIKE_G(global_error), code, msg)
#define UPDATE_CURRENT_ERROR(code, msg) update_client_error(getThis(), code, msg)

#define RESET_CURRENT_ERROR() as_error_set_message(&AEROSPIKE_G(global_error), 0, "") // sets code to AEROSPIKE_OK
#endif	/* PHP_AEROSPIKE_H */

