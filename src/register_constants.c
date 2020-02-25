/*constants.c*/
#include "register_constants.h"
#include "aerospike/as_status.h"
#include "aerospike/aerospike_index.h"
#include "aerospike/as_operations.h"
#include "php_aerospike_types.h"
//int zend_declare_class_constant_long ( zend_class_entry* ce, char* name, size_t name_length, long value, magic TSRMLS_DC 

#define AEROSPIKE_STATUS_ARR_SIZE (sizeof(aerospike_status)/sizeof(AerospikeStatus))
#define AEROSPIKE_GENERAL_LONG_CONSTANTS_ARR_SIZE (sizeof(aerospike_general_long_constants)/sizeof(AerospikeGeneralLongConstant))
#define AEROSPIKE_GENERAL_STRING_CONSTANTS_ARR_SIZE (sizeof(aerospike_general_string_constants)/sizeof(AerospikeGeneralStringConstant))

/*
 *******************************************************************************************************
 * Structure to map constant number to constant name string for Aerospike status constants.
 *******************************************************************************************************
 */
typedef struct Aerospike_Status {
	int statusno;
	char status_msg[MAX_STATUS_MSG_SIZE];
} AerospikeStatus;

/*
 *******************************************************************************************************
 * Structure to map constant number to constant name string for Aerospike General Long constants.
 *******************************************************************************************************
 */
typedef struct Aerospike_General_Long_Constants {
	int constant_value;
	char constant_str[MAX_GENERAL_CONSTANT_STR_SIZE];
} AerospikeGeneralLongConstant;

/*
 *******************************************************************************************************
 * Structure to map constant number to constant name string for Aerospike General String constants.
 *******************************************************************************************************
 */
typedef struct Aerospike_General_String_Constants {
	char constant_value[MAX_GENERAL_CONSTANT_STR_SIZE];
	char constant_str[MAX_GENERAL_CONSTANT_STR_SIZE];
} AerospikeGeneralStringConstant;

/**
 ** Function which takes the aerospike class entry, and adds all of the status codes to it
*/
static AerospikeGeneralLongConstant aerospike_general_long_constants[] = {
	{ AS_INDEX_STRING,                 "INDEX_STRING"                 },
	{ AS_INDEX_NUMERIC,                "INDEX_NUMERIC"                },
	{ AS_INDEX_TYPE_DEFAULT,           "INDEX_TYPE_DEFAULT"           },
	{ AS_INDEX_TYPE_LIST,              "INDEX_TYPE_LIST"              },
	{ AS_INDEX_TYPE_MAPKEYS,           "INDEX_TYPE_MAPKEYS"           },
	{ AS_INDEX_TYPE_MAPVALUES,         "INDEX_TYPE_MAPVALUES"         },
	{ AS_OPERATOR_WRITE,               "OPERATOR_WRITE"               },
	{ AS_OPERATOR_READ,                "OPERATOR_READ"                },
	{ AS_OPERATOR_INCR,                "OPERATOR_INCR"                },
	{ AS_OPERATOR_PREPEND,             "OPERATOR_PREPEND"             },
	{ AS_OPERATOR_APPEND,              "OPERATOR_APPEND"              },
	{ AS_OPERATOR_TOUCH,               "OPERATOR_TOUCH"               },
	{ AS_OPERATOR_DELETE,              "OPERATOR_DELETE"              },
	{ AS_INDEX_GEO2DSPHERE,            "INDEX_GEO2DSPHERE"            },
	{ OP_LIST_APPEND,       		   "OP_LIST_APPEND"               },
	{ OP_LIST_MERGE,       		       "OP_LIST_MERGE"                },
	{ OP_LIST_INSERT,       		   "OP_LIST_INSERT"               },
	{ OP_LIST_INSERT_ITEMS, 		   "OP_LIST_INSERT_ITEMS"         },
	{ OP_LIST_POP,          		   "OP_LIST_POP"                  },
	{ OP_LIST_POP_RANGE,    		   "OP_LIST_POP_RANGE"            },
	{ OP_LIST_REMOVE,       		   "OP_LIST_REMOVE"               },
	{ OP_LIST_REMOVE_RANGE, 		   "OP_LIST_REMOVE_RANGE"         },
	{ OP_LIST_CLEAR,        		   "OP_LIST_CLEAR"                },
	{ OP_LIST_SET,         			   "OP_LIST_SET"                  },
	{ OP_LIST_GET,          		   "OP_LIST_GET"                  },
	{ OP_LIST_GET_RANGE,    		   "OP_LIST_GET_RANGE"            },
	{ OP_LIST_TRIM,         		   "OP_LIST_TRIM"                 },
	{ OP_LIST_SIZE,         		   "OP_LIST_SIZE"                 },
	{ OP_MAP_SET_POLICY,               "OP_MAP_SET_POLICY"            },
	{ OP_MAP_PUT,                      "OP_MAP_PUT"                   },
	{ OP_MAP_PUT_ITEMS,                "OP_MAP_PUT_ITEMS"             },
	{ OP_MAP_INCREMENT,                "OP_MAP_INCREMENT"             },
	{ OP_MAP_DECREMENT,                "OP_MAP_DECREMENT"             },
	{ OP_MAP_SIZE,                     "OP_MAP_SIZE"                  },
	{ OP_MAP_CLEAR,                    "OP_MAP_CLEAR"                 },
	{ OP_MAP_REMOVE_BY_KEY,            "OP_MAP_REMOVE_BY_KEY"         },
	{ OP_MAP_REMOVE_BY_KEY_LIST,       "OP_MAP_REMOVE_BY_KEY_LIST"    },
	{ OP_MAP_REMOVE_BY_KEY_RANGE,      "OP_MAP_REMOVE_BY_KEY_RANGE"   },
	{ OP_MAP_REMOVE_BY_VALUE,          "OP_MAP_REMOVE_BY_VALUE"       },
	{ OP_MAP_REMOVE_BY_VALUE_LIST,     "OP_MAP_REMOVE_BY_VALUE_LIST"  },
	{ OP_MAP_REMOVE_BY_VALUE_RANGE,    "OP_MAP_REMOVE_BY_VALUE_RANGE" },
	{ OP_MAP_REMOVE_BY_INDEX,          "OP_MAP_REMOVE_BY_INDEX"       },
	{ OP_MAP_REMOVE_BY_INDEX_RANGE,    "OP_MAP_REMOVE_BY_INDEX_RANGE" },
	{ OP_MAP_REMOVE_BY_RANK,           "OP_MAP_REMOVE_BY_RANK"        },
	{ OP_MAP_REMOVE_BY_RANK_RANGE,     "OP_MAP_REMOVE_BY_RANK_RANGE"  },
	{ OP_MAP_GET_BY_KEY,               "OP_MAP_GET_BY_KEY"            },
	{ OP_MAP_GET_BY_KEY_RANGE,         "OP_MAP_GET_BY_KEY_RANGE"      },
	{ OP_MAP_GET_BY_VALUE,             "OP_MAP_GET_BY_VALUE"          },
	{ OP_MAP_GET_BY_VALUE_RANGE,       "OP_MAP_GET_BY_VALUE_RANGE"    },
	{ OP_MAP_GET_BY_INDEX,             "OP_MAP_GET_BY_INDEX"          },
	{ OP_MAP_GET_BY_INDEX_RANGE,       "OP_MAP_GET_BY_INDEX_RANGE"    },
	{ OP_MAP_GET_BY_RANK,              "OP_MAP_GET_BY_RANK"           },
	{ OP_MAP_GET_BY_RANK_RANGE,        "OP_MAP_GET_BY_RANK_RANGE"     }
};

static AerospikeStatus aerospike_status[] = {
		{ AEROSPIKE_ERR_CONNECTION                ,   "ERR_CONNECTION"                     },
		{ AEROSPIKE_ERR_TLS_ERROR                 ,   "ERR_TLS"                            },
		{ AEROSPIKE_ERR_INVALID_NODE              ,   "ERR_INVALID_NODE"                   },
		{ AEROSPIKE_ERR_NO_MORE_CONNECTIONS       ,   "ERR_NO_MORE_CONNECTIONS"            },
		{ AEROSPIKE_ERR_ASYNC_CONNECTION          ,   "ERR_ASYNC_CONNECTION"               },
		{ AEROSPIKE_ERR_CLIENT_ABORT              ,   "ERR_CLIENT_ABORT"                   },
		{ AEROSPIKE_ERR_INVALID_HOST              ,   "ERR_INVALID_HOST"                   },
		{ AEROSPIKE_ERR_PARAM                     ,   "ERR_PARAM"                          },
		{ AEROSPIKE_ERR_CLIENT                    ,   "ERR_CLIENT"                         },
		{ AEROSPIKE_OK                            ,   "OK"                                 },
		{ AEROSPIKE_ERR_SERVER                    ,   "ERR_SERVER"                         },
		{ AEROSPIKE_ERR_RECORD_NOT_FOUND          ,   "ERR_RECORD_NOT_FOUND"               },
		{ AEROSPIKE_ERR_RECORD_GENERATION         ,   "ERR_RECORD_GENERATION"              },
		{ AEROSPIKE_ERR_REQUEST_INVALID           ,   "ERR_REQUEST_INVALID"                },
		{ AEROSPIKE_ERR_OP_NOT_APPLICABLE         ,   "ERR_OP_NOT_APPLICABLE"              },
		{ AEROSPIKE_ERR_RECORD_EXISTS             ,   "ERR_RECORD_EXISTS"                  },
		{ AEROSPIKE_ERR_CLUSTER_CHANGE            ,   "ERR_CLUSTER_CHANGE"                 },
		{ AEROSPIKE_ERR_SERVER_FULL               ,   "ERR_SERVER_FULL"                    },
		{ AEROSPIKE_ERR_TIMEOUT                   ,   "ERR_TIMEOUT"                        },
		{ AEROSPIKE_ERR_ALWAYS_FORBIDDEN          ,   "ERR_ALWAYS_FORBIDDEN"               },
		{ AEROSPIKE_ERR_CLUSTER                   ,   "ERR_CLUSTER"                        },
		{ AEROSPIKE_ERR_BIN_INCOMPATIBLE_TYPE     ,   "ERR_BIN_INCOMPATIBLE_TYPE"          },
		{ AEROSPIKE_ERR_RECORD_TOO_BIG            ,   "ERR_RECORD_TOO_BIG"                 },
		{ AEROSPIKE_ERR_RECORD_BUSY               ,   "ERR_RECORD_BUSY"                    },
		{ AEROSPIKE_ERR_SCAN_ABORTED              ,   "ERR_SCAN_ABORTED"                   },
		{ AEROSPIKE_ERR_UNSUPPORTED_FEATURE       ,   "ERR_UNSUPPORTED_FEATURE"            },
		{ AEROSPIKE_ERR_DEVICE_OVERLOAD           ,   "ERR_DEVICE_OVERLOAD"                },
		{ AEROSPIKE_ERR_RECORD_KEY_MISMATCH       ,   "ERR_RECORD_KEY_MISMATCH"            },
		{ AEROSPIKE_ERR_NAMESPACE_NOT_FOUND       ,   "ERR_NAMESPACE_NOT_FOUND"            },
		{ AEROSPIKE_ERR_BIN_NAME                  ,   "ERR_BIN_NAME"                       },
		{ AEROSPIKE_ERR_FAIL_FORBIDDEN            ,   "ERR_FORBIDDEN"                      },
		{ AEROSPIKE_ERR_FAIL_ELEMENT_NOT_FOUND    ,   "ERR_FAIL_NOT_FOUND"                 },
		{ AEROSPIKE_ERR_FAIL_ELEMENT_EXISTS       ,   "ERR_FAIL_ELEMENT_EXISTS"            },
		{ AEROSPIKE_QUERY_END                     ,   "ERR_QUERY_END"                      },
		{ AEROSPIKE_ERR_UDF                       ,   "ERR_UDF"                            },
		{ AEROSPIKE_ERR_BATCH_DISABLED            ,   "ERR_BATCH_DISABLED"                 },
		{AEROSPIKE_ERR_BATCH_MAX_REQUESTS_EXCEEDED,   "ERR_BATCH_MAX_REQUESTS_EXCEEDED"    },
		{ AEROSPIKE_ERR_BATCH_QUEUES_FULL         ,   "ERR_BATCH_QUEUES_FULL "             },
		{ AEROSPIKE_ERR_INDEX_FOUND               ,   "ERR_INDEX_FOUND"                    },
		{ AEROSPIKE_ERR_INDEX_NOT_FOUND           ,   "ERR_INDEX_NOT_FOUND"                },
		{ AEROSPIKE_ERR_INDEX_OOM                 ,   "ERR_INDEX_OOM"                      },
		{ AEROSPIKE_ERR_INDEX_NOT_READABLE        ,   "ERR_INDEX_NOT_READABLE"             },
		{ AEROSPIKE_ERR_INDEX                     ,   "ERR_INDEX"                          },
		{ AEROSPIKE_ERR_INDEX_NAME_MAXLEN         ,   "ERR_INDEX_NAME_MAXLEN"              },
		{ AEROSPIKE_ERR_INDEX_MAXCOUNT            ,   "ERR_INDEX_MAXCOUNT"                 },
		{ AEROSPIKE_ERR_QUERY_ABORTED             ,   "ERR_QUERY_ABORTED"                  },
		{ AEROSPIKE_ERR_QUERY_QUEUE_FULL          ,   "ERR_QUERY_QUEUE_FULL"               },
		{ AEROSPIKE_ERR_QUERY_TIMEOUT             ,   "ERR_QUERY_TIMEOUT"                  },
		{ AEROSPIKE_ERR_QUERY                     ,   "ERR_QUERY"                          },
		{ AEROSPIKE_ERR_UDF_NOT_FOUND             ,   "ERR_UDF_NOT_FOUND"                  },
		{ AEROSPIKE_ERR_LUA_FILE_NOT_FOUND        ,   "ERR_LUA_FILE_NOT_FOUND"             },
		{ AEROSPIKE_SECURITY_NOT_SUPPORTED        ,   "ERR_SECURITY_NOT_SUPPORTED"         },
		{ AEROSPIKE_SECURITY_NOT_ENABLED          ,   "ERR_SECURITY_NOT_ENABLED"           },
		{ AEROSPIKE_SECURITY_SCHEME_NOT_SUPPORTED ,   "ERR_SECURITY_SCHEME_NOT_SUPPORTED"  },
		{ AEROSPIKE_INVALID_USER                  ,   "ERR_INVALID_USER"                   },
		{ AEROSPIKE_USER_ALREADY_EXISTS           ,   "ERR_USER_ALREADY_EXISTS"            },
		{ AEROSPIKE_INVALID_PASSWORD              ,   "ERR_INVALID_PASSWORD"               },
		{ AEROSPIKE_EXPIRED_PASSWORD              ,   "ERR_EXPIRED_PASSWORD"               },
		{ AEROSPIKE_FORBIDDEN_PASSWORD            ,   "ERR_FORBIDDEN_PASSWORD"             },
		{ AEROSPIKE_INVALID_CREDENTIAL            ,   "ERR_INVALID_CREDENTIAL"             },
		{ AEROSPIKE_INVALID_ROLE                  ,   "ERR_INVALID_ROLE"                   },
		{ AEROSPIKE_INVALID_PRIVILEGE             ,   "ERR_INVALID_PRIVILEGE"              },
		{ AEROSPIKE_INVALID_COMMAND               ,   "ERR_INVALID_COMMAND"                },
		{ AEROSPIKE_INVALID_FIELD                 ,   "ERR_INVALID_FIELD"                  },
		{ AEROSPIKE_ILLEGAL_STATE                 ,   "ERR_ILLEGAL_STATE"                  },
		{ AEROSPIKE_NOT_AUTHENTICATED             ,   "ERR_NOT_AUTHENTICATED"              },
		{ AEROSPIKE_ROLE_VIOLATION                ,   "ERR_ROLE_VIOLATION"                 },
		{ AEROSPIKE_ROLE_ALREADY_EXISTS           ,   "ERR_ROLE_ALREADY_EXISTS"            },
		{ AEROSPIKE_ERR_GEO_INVALID_GEOJSON       ,   "ERR_GEO_INVALID_GEOJSON"            }
	};

static AerospikeGeneralStringConstant aerospike_general_string_constants[] = {
	{ "=",           "OP_EQ"               },
	{ "BETWEEN",     "OP_BETWEEN"          },
	{ "CONTAINS",    "OP_CONTAINS"         },
	{ "RANGE",       "OP_RANGE"            },
	{ "GEOWITHIN",   "OP_GEOWITHINREGION"  },
	{ "GEOCONTAINS", "OP_GEOCONTAINSPOINT" },
	{ "query",       "JOB_QUERY"           },
	{ "scan",        "JOB_SCAN"            }
};

bool register_aerospike_status_codes(zend_class_entry* aerospike_ce) {
	int i;
	AerospikeStatus current_status;
	for(i = 0; i < AEROSPIKE_STATUS_ARR_SIZE; i++) {
		current_status = aerospike_status[i];
		zend_declare_class_constant_long(aerospike_ce,
										 current_status.status_msg, strlen(current_status.status_msg),
										 current_status.statusno);
	}
	return true;
}

bool register_aerospike_class_constants(zend_class_entry* aerospike_ce) {
	int i;
	AerospikeGeneralStringConstant current_string_constant;
	AerospikeGeneralLongConstant current_long_constant;

	for(i = 0; i < AEROSPIKE_GENERAL_STRING_CONSTANTS_ARR_SIZE; i++) {
		current_string_constant = aerospike_general_string_constants[i];
		zend_declare_class_constant_string(
			aerospike_ce,
			current_string_constant.constant_str,
			strlen(current_string_constant.constant_str),
			current_string_constant.constant_value
		);
	}

	for(i = 0; i < AEROSPIKE_GENERAL_LONG_CONSTANTS_ARR_SIZE; i++) {
		current_long_constant = aerospike_general_long_constants[i];
		zend_declare_class_constant_long(
			aerospike_ce,
			current_long_constant.constant_str,
			strlen(current_long_constant.constant_str),
			aerospike_general_long_constants[i].constant_value
		);
	}
	return true;
}
