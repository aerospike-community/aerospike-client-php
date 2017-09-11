#include "aerospike_class.h"
#include "php_aerospike_types.h"
#include "logging.h"
#include "aerospike/as_log.h"


/* Constants used by loggers */
#define MAX_LOGGER_CONSTANT_STR_SIZE 512


/*
 *******************************************************************************************************
 * Structure to map constant number to constant name string for Aerospike Logger constants.
 *******************************************************************************************************
 */
typedef struct Aerospike_logger_Constants {
	int constantno;
	char constant_str[MAX_LOGGER_CONSTANT_STR_SIZE];
} AerospikeLoggerConstants;

/*
 *******************************************************************************************************
 * Instance of Mapper of constant number to constant name string for Aerospike Logger constants.
 *******************************************************************************************************
 */
AerospikeLoggerConstants aerospike_logger_constants[] = {
	{ AS_LOG_LEVEL_TRACE, "LOG_LEVEL_TRACE" },
	{ AS_LOG_LEVEL_DEBUG, "LOG_LEVEL_DEBUG" },
	{ AS_LOG_LEVEL_INFO,  "LOG_LEVEL_INFO"  },
	{ AS_LOG_LEVEL_WARN,  "LOG_LEVEL_WARN"  },
	{ AS_LOG_LEVEL_ERROR, "LOG_LEVEL_ERROR" },
};

#define AEROSPIKE_LOGGER_CONSTANTS_ARR_SIZE (sizeof(aerospike_logger_constants)/sizeof(AerospikeLoggerConstants))

/*
 *******************************************************************************************************
 * Function to expose logger constants
 *
 * @param Aerospike_ce          The zend class entry for Aerospike class.
 *******************************************************************************************************
 */

bool register_logger_constants(zend_class_entry* aerospike_ce) {
	int32_t i;
	int status = 1;
	for (i = 0; i < AEROSPIKE_LOGGER_CONSTANTS_ARR_SIZE; i++) {
		status = zend_declare_class_constant_long(
					aerospike_ce, aerospike_logger_constants[i].constant_str,
					strlen(aerospike_logger_constants[i].constant_str),
					aerospike_logger_constants[i].constantno);
		/* If the loading of a constant failed, return */
		if (status != SUCCESS) {
			return false;
		}
	}
	return true;
}
