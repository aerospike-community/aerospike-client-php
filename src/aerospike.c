
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_aerospike.h"
#include "aerospike_session.h"

// Aerospike PHP client includes
#include "php_aerospike_types.h"
#include "aerospike_class.h"
#include "persistent_list.h"
// #include "include/constants.h"


// Load the global entries
ZEND_DECLARE_MODULE_GLOBALS(aerospike)

/* True global resources - no need for thread safety here */
static int le_aerospike;


/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("aerospike.nesting_depth", "3", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, nesting_depth, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.connect_timeout", "1000", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, connect_timeout, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.read_timeout", "1000", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, read_timeout, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.write_timeout", "1000", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, write_timeout, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.log_path", NULL, PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateString, log_path, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.log_level", NULL, PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateString, log_level, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.serializer", "1", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong , serializer, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.udf.lua_user_path", "/usr/local/aerospike/usr-lua", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateString, lua_user_path, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.key_policy", "0", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, key_policy, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.key_gen", "0", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, key_gen, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.shm.use", "false", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateBool, shm_use, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.shm.key", "0xA8000000", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, shm_key, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.shm.max_nodes", "16", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, shm_max_nodes, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.shm.max_namespaces", "8", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, shm_max_namespaces, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.shm.takeover_threshold_sec", "30", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, shm_takeover_threshold_sec, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.max_threads", "300", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, max_threads, zend_aerospike_globals, aerospike_globals)
	// This causes issues consider removal
    STD_PHP_INI_ENTRY("aerospike.thread_pool_size", "16", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, thread_pool_size, zend_aerospike_globals, aerospike_globals)
    STD_PHP_INI_ENTRY("aerospike.compression_threshold", "0", PHP_INI_PERDIR|PHP_INI_SYSTEM|PHP_INI_USER, OnUpdateLong, compression_threshold, zend_aerospike_globals, aerospike_globals)
PHP_INI_END()

/* }}} */




/* {{{ php_aerospike_init_globals
 */
/* Uncomment this function if you have global entries
static void php_aerospike_init_globals(zend_aerospike_globals *aerospike_globals)
{
	aerospike_globals->global_value = 0;
	aerospike_globals->global_string = NULL;
}
*/
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */

PHP_GINIT_FUNCTION(aerospike) {
#if defined(COMPILE_DL_AEROSPIKE) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif


	pthread_mutex_init(&aerospike_globals->query_cb_mutex, NULL);
	as_error_init(&(aerospike_globals->global_error));
	aerospike_globals->is_global_user_deserializer_registered = false;
	memset(&aerospike_globals->user_global_deserializer_call_info, 0, sizeof(zend_fcall_info));
	memset(&aerospike_globals->user_global_deserializer_call_info_cache, 0, sizeof(zend_fcall_info_cache));

	aerospike_globals->is_global_user_serializer_registered = false;
	memset(&aerospike_globals->user_global_serializer_call_info, 0, sizeof(zend_fcall_info));
	memset(&aerospike_globals->user_global_serializer_call_info_cache, 0, sizeof(zend_fcall_info_cache));

	aerospike_globals->is_log_callback_registered = false;
	memset(&aerospike_globals->log_callback_call_info, 0, sizeof(zend_fcall_info));
	memset(&aerospike_globals->log_callback_call_info_cache, 0, sizeof(zend_fcall_info_cache));

	/* Create the global host list */
	aerospike_globals->persistent_list_g = (HashTable*)pemalloc(sizeof(HashTable), 1);
}

PHP_GSHUTDOWN_FUNCTION(aerospike) {
    zend_hash_destroy(aerospike_globals->persistent_list_g);
    pefree(aerospike_globals->persistent_list_g, 1);
	return;
}

PHP_MINIT_FUNCTION(aerospike)
{

	REGISTER_INI_ENTRIES();
	/* initialize the hash table */
	zend_hash_init(AEROSPIKE_G(persistent_list_g), 8, NULL, persistent_host_dtor, 1);


	register_aerospike_class();
	php_session_register_module(&ps_mod_aerospike);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(aerospike)
{
	/* uncomment this line if you have INI entries*/
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(aerospike)
{
	as_error_init(&AEROSPIKE_G(global_error));

	memset(&AEROSPIKE_G(user_global_serializer_call_info), 0, sizeof(zend_fcall_info));
	memset(&AEROSPIKE_G(user_global_deserializer_call_info), 0, sizeof(zend_fcall_info));

	memset(&AEROSPIKE_G(user_global_serializer_call_info_cache), 0, sizeof(zend_fcall_info));
	memset(&AEROSPIKE_G(user_global_deserializer_call_info_cache), 0, sizeof(zend_fcall_info));

	memset(&AEROSPIKE_G(log_callback_call_info), 0, sizeof(zend_fcall_info));
	memset(&AEROSPIKE_G(log_callback_call_info_cache), 0, sizeof(zend_fcall_info_cache));

	AEROSPIKE_G(is_global_user_deserializer_registered) = false;
	AEROSPIKE_G(is_global_user_serializer_registered) = false;
	AEROSPIKE_G(is_log_callback_registered) = false;

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(aerospike)
{
	if AEROSPIKE_G(is_global_user_serializer_registered) {
		zval_dtor(&AEROSPIKE_G(user_global_serializer_call_info).function_name);
	}

	if AEROSPIKE_G(is_global_user_deserializer_registered) {
		zval_dtor(&AEROSPIKE_G(user_global_deserializer_call_info).function_name);
	}

	if AEROSPIKE_G(is_log_callback_registered) {
		zval_dtor(&AEROSPIKE_G(log_callback_call_info).function_name);
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(aerospike)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "aerospike support", "enabled");
    php_info_print_table_header(2, "client version", PHP_AEROSPIKE_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ aerospike_functions[]
 *
 * Every user visible function must have an entry in aerospike_functions[].
 */
const zend_function_entry aerospike_functions[] = {
	//PHP_FE(confirm_aerospike_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in aerospike_functions[] */
};
/* }}} */

/* {{{ aerospike_module_entry
 */
zend_module_entry aerospike_module_entry = {
	STANDARD_MODULE_HEADER,
	"aerospike",
	aerospike_functions,
	PHP_MINIT(aerospike),
	PHP_MSHUTDOWN(aerospike),
	PHP_RINIT(aerospike),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(aerospike),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(aerospike),
	PHP_AEROSPIKE_VERSION,
	PHP_MODULE_GLOBALS(aerospike), /*globals information*/
	PHP_GINIT(aerospike), /*globals constructor */
	PHP_GSHUTDOWN(aerospike), /* globals destructor */
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */
#ifdef COMPILE_DL_AEROSPIKE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(aerospike)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
