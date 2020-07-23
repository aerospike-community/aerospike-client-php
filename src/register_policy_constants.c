#include "register_policy_constants.h"
#include "php_aerospike_types.h"
#include "aerospike/as_status.h"
#include "aerospike/aerospike_index.h"
#include "php_aerospike_types.h"
#include "aerospike/as_scan.h"
#include "aerospike/as_job.h"
#include "aerospike/as_admin.h"
#include "aerospike/as_operations.h"
#include "aerospike/as_map_operations.h"

#define AEROSPIKE_OPTION_CONSTANTS_ARR_SIZE (sizeof(aerospike_option_constants)/sizeof(AerospikeOptionConstant))
#define AEROSPIKE_OPTION_STRCONSTANTS_ARR_SIZE (sizeof(aerospike_str_option_constants)/sizeof(AerospikeStrOptionConstant))

#define MAX_CONSTANT_STR_SIZE 512

typedef struct _AerospikeOptionConstant {
	long constant_value;
	char constant_str[MAX_CONSTANT_STR_SIZE];
} AerospikeOptionConstant;

typedef struct _AerospikeStrOptionConstant {
	/* Value of the constant */
	const char constant_str_value[MAX_CONSTANT_STR_SIZE];
	/* name of the constant Aerospike::policy_str */
	char constant_name[MAX_CONSTANT_STR_SIZE];
}AerospikeStrOptionConstant;

/**
 ** Function which takes the aerospike class entry, and adds all of the status codes to it*/

static
AerospikeOptionConstant aerospike_option_constants[] = {
	{ OPT_CONNECT_TIMEOUT                   ,   "OPT_CONNECT_TIMEOUT"               },
	{ OPT_READ_TIMEOUT                      ,   "OPT_READ_TIMEOUT"                  },
	{ OPT_WRITE_TIMEOUT                     ,   "OPT_WRITE_TIMEOUT"                 },
	{ OPT_POLICY_RETRY                      ,   "OPT_POLICY_RETRY"                  },
	{ OPT_POLICY_EXISTS                     ,   "OPT_POLICY_EXISTS"                 },
	{ OPT_POLICY_KEY                        ,   "OPT_POLICY_KEY"                    },
	{ OPT_SERIALIZER                        ,   "OPT_SERIALIZER"                    },
	{ OPT_SCAN_PRIORITY                     ,   "OPT_SCAN_PRIORITY"                 },
	{ OPT_SCAN_RPS_LIMIT                    ,   "OPT_SCAN_RPS_LIMIT"                },
	{ OPT_SCAN_PERCENTAGE                   ,   "OPT_SCAN_PERCENTAGE"               },
	{ OPT_SCAN_CONCURRENTLY                 ,   "OPT_SCAN_CONCURRENTLY"             },
	{ OPT_SCAN_NOBINS                       ,   "OPT_SCAN_NOBINS"                   },
	{ OPT_SCAN_INCLUDELDT                   ,   "OPT_SCAN_INCLUDELDT"               },
	{ OPT_POLICY_GEN                        ,   "OPT_POLICY_GEN"                    },
	{ OPT_POLICY_REPLICA                    ,   "OPT_POLICY_REPLICA"                },
	{ OPT_POLICY_READ_MODE_AP               ,   "OPT_POLICY_READ_MODE_AP"           },
	{ OPT_POLICY_READ_MODE_SC               ,   "OPT_POLICY_READ_MODE_SC"           },
	{ OPT_POLICY_COMMIT_LEVEL               ,   "OPT_POLICY_COMMIT_LEVEL"           },
	{ OPT_TTL                               ,   "OPT_TTL"                           },
	{ USE_SERVICES_ALTERNATE                ,   "USE_SERVICES_ALTERNATE"            },
	{ USE_BATCH_DIRECT                      ,   "USE_BATCH_DIRECT"                  },
	{ COMPRESSION_THRESHOLD                 ,   "COMPRESSION_THRESHOLD"             },
	{ AS_POLICY_RETRY_NONE                  ,   "POLICY_RETRY_NONE"                 },
	{ AS_POLICY_RETRY_ONCE                  ,   "POLICY_RETRY_ONCE"                 },
	{ AS_POLICY_EXISTS_IGNORE               ,   "POLICY_EXISTS_IGNORE"              },
	{ AS_POLICY_EXISTS_CREATE               ,   "POLICY_EXISTS_CREATE"              },
	{ AS_POLICY_EXISTS_UPDATE               ,   "POLICY_EXISTS_UPDATE"              },
	{ AS_POLICY_EXISTS_REPLACE              ,   "POLICY_EXISTS_REPLACE"             },
	{ AS_POLICY_EXISTS_CREATE_OR_REPLACE    ,   "POLICY_EXISTS_CREATE_OR_REPLACE"   },
	{ SERIALIZER_NONE                       ,   "SERIALIZER_NONE"                   },
	{ SERIALIZER_PHP                        ,   "SERIALIZER_PHP"                    },
	{ SERIALIZER_USER                       ,   "SERIALIZER_USER"                   },
	{ AS_UDF_TYPE_LUA                       ,   "UDF_TYPE_LUA"                      },
	{ AS_SCAN_PRIORITY_AUTO                 ,   "SCAN_PRIORITY_AUTO"                },
	{ AS_SCAN_PRIORITY_LOW                  ,   "SCAN_PRIORITY_LOW"                 },
	{ AS_SCAN_PRIORITY_MEDIUM               ,   "SCAN_PRIORITY_MEDIUM"              },
	{ AS_SCAN_PRIORITY_HIGH                 ,   "SCAN_PRIORITY_HIGH"                },
	{ AS_SCAN_STATUS_UNDEF                  ,   "SCAN_STATUS_UNDEF"                 },
	{ AS_SCAN_STATUS_INPROGRESS             ,   "SCAN_STATUS_INPROGRESS"            },
	{ AS_SCAN_STATUS_ABORTED                ,   "SCAN_STATUS_ABORTED"               },
	{ AS_SCAN_STATUS_COMPLETED              ,   "SCAN_STATUS_COMPLETED"             },
	{ AS_JOB_STATUS_UNDEF                   ,   "JOB_STATUS_UNDEF"                  },
	{ AS_JOB_STATUS_INPROGRESS              ,   "JOB_STATUS_INPROGRESS"             },
	{ AS_JOB_STATUS_COMPLETED               ,   "JOB_STATUS_COMPLETED"              },
	{ AS_POLICY_KEY_DIGEST                  ,   "POLICY_KEY_DIGEST"                 },
	{ AS_POLICY_KEY_SEND                    ,   "POLICY_KEY_SEND"                   },
	{ AS_POLICY_GEN_IGNORE                  ,   "POLICY_GEN_IGNORE"                 },
	{ AS_POLICY_GEN_EQ                      ,   "POLICY_GEN_EQ"                     },
	{ AS_POLICY_GEN_GT                      ,   "POLICY_GEN_GT"                     },
	{ AS_POLICY_REPLICA_MASTER              ,   "POLICY_REPLICA_MASTER"             },
	{ AS_POLICY_REPLICA_ANY                 ,   "POLICY_REPLICA_ANY"                },
	{ AS_POLICY_REPLICA_SEQUENCE            ,   "POLICY_REPLICA_SEQUENCE"           },
    { AS_POLICY_REPLICA_PREFER_RACK         ,   "POLICY_REPLICA_PREFER_RACK"        },
	{ AS_POLICY_READ_MODE_AP_ONE	        ,   "POLICY_READ_MODE_AP_ONE"           },
	{ AS_POLICY_READ_MODE_AP_ALL     	    ,   "POLICY_READ_MODE_AP_ALL"           },
	{ AS_POLICY_READ_MODE_SC_SESSION        ,   "POLICY_READ_MODE_SC_SESSION"       },
	{ AS_POLICY_READ_MODE_SC_LINEARIZE      ,   "POLICY_READ_MODE_SC_LINEARIZE"     },
	{ AS_POLICY_READ_MODE_SC_ALLOW_REPLICA  ,   "POLICY_READ_MODE_SC_ALLOW_REPLICA" },
	{ AS_POLICY_READ_MODE_SC_ALLOW_UNAVAILABLE , "POLICY_READ_MODE_SC_ALLOW_UNAVAILABLE"},
	{ AS_POLICY_COMMIT_LEVEL_ALL            ,   "POLICY_COMMIT_LEVEL_ALL"           },
	{ AS_POLICY_COMMIT_LEVEL_MASTER         ,   "POLICY_COMMIT_LEVEL_MASTER"        },
	{ AS_PRIVILEGE_USER_ADMIN               ,   "PRIV_USER_ADMIN"                   },
	{ AS_PRIVILEGE_SYS_ADMIN                ,   "PRIV_SYS_ADMIN"                    },
	{ AS_PRIVILEGE_READ                     ,   "PRIV_READ"                         },
	{ AS_PRIVILEGE_READ_WRITE               ,   "PRIV_READ_WRITE"                   },
	{ AS_PRIVILEGE_READ_WRITE_UDF           ,   "PRIV_READ_WRITE_UDF"               },
	{ AS_PRIVILEGE_DATA_ADMIN               ,   "PRIV_DATA_ADMIN"                   },
	{ OPT_POLICY_DURABLE_DELETE             ,   "OPT_POLICY_DURABLE_DELETE"         },
	{ OPT_SOCKET_TIMEOUT                    ,   "OPT_SOCKET_TIMEOUT"                },
	{ OPT_MAP_ORDER                         ,   "OPT_MAP_ORDER"                     },
	{ OPT_MAP_WRITE_MODE                    ,   "OPT_MAP_WRITE_MODE"                },
	{ OPT_MAP_WRITE_FLAGS                   ,   "OPT_MAP_WRITE_FLAGS"               },
	{ AS_MAP_UNORDERED                      ,   "AS_MAP_UNORDERED"                  },
	{ AS_MAP_KEY_ORDERED                    ,   "AS_MAP_KEY_ORDERED"                },
	{ AS_MAP_KEY_VALUE_ORDERED              ,   "AS_MAP_KEY_VALUE_ORDERED"          },
	{ AS_MAP_UPDATE                         ,   "AS_MAP_UPDATE"                     },
	{ AS_MAP_UPDATE_ONLY                    ,   "AS_MAP_UPDATE_ONLY"                },
	{ AS_MAP_CREATE_ONLY                    ,   "AS_MAP_CREATE_ONLY"                },
	{ AS_MAP_WRITE_DEFAULT                  ,   "AS_MAP_WRITE_DEFAULT"              },
	{ AS_MAP_WRITE_CREATE_ONLY              ,   "AS_MAP_WRITE_CREATE_ONLY"          },
	{ AS_MAP_WRITE_UPDATE_ONLY              ,   "AS_MAP_WRITE_UPDATE_ONLY"          },
	{ AS_MAP_WRITE_NO_FAIL                  ,   "AS_MAP_WRITE_NO_FAIL"              },
	{ AS_MAP_WRITE_PARTIAL                  ,   "AS_MAP_WRITE_PARTIAL"              },
	{AS_MAP_RETURN_NONE                     ,   "MAP_RETURN_NONE"                   },
	{AS_MAP_RETURN_INDEX                    ,   "MAP_RETURN_INDEX"                  },
	{AS_MAP_RETURN_REVERSE_INDEX            ,   "MAP_RETURN_REVERSE_INDEX"          },
	{AS_MAP_RETURN_RANK                     ,   "MAP_RETURN_RANK"                   },
	{AS_MAP_RETURN_REVERSE_RANK             ,   "MAP_RETURN_REVERSE_RANK"           },
	{AS_MAP_RETURN_COUNT                    ,   "MAP_RETURN_COUNT"                  },
	{AS_MAP_RETURN_KEY                      ,   "MAP_RETURN_KEY"                    },
	{AS_MAP_RETURN_VALUE                    ,   "MAP_RETURN_VALUE"                  },
	{AS_MAP_RETURN_KEY_VALUE                ,   "MAP_RETURN_KEY_VALUE"              },
	{ OPT_TOTAL_TIMEOUT                     ,   "OPT_TOTAL_TIMEOUT"                 },
	{ OPT_MAX_RETRIES                       ,   "OPT_MAX_RETRIES"                   },
	{ OPT_FAIL_ON_CLUSTER_CHANGE            ,   "OPT_FAIL_ON_CLUSTER_CHANGE"        },
	{ OPT_BATCH_CONCURRENT                  ,   "OPT_BATCH_CONCURRENT"              },
	{ OPT_ALLOW_INLINE                      ,   "OPT_ALLOW_INLINE"                  },
	{ OPT_SEND_SET_NAME                     ,   "OPT_SEND_SET_NAME"                 },
	{OPT_TLS_CONFIG                         ,   "OPT_TLS_CONFIG"                    },
	{OPT_TLS_ENABLE                         ,   "OPT_TLS_ENABLE"                    },
	{OPT_TLS_CAFILE                         ,   "OPT_TLS_CAFILE"                    },
	{OPT_TLS_CAPATH                         ,   "OPT_TLS_CAPATH"                    },
	{OPT_TLS_PROTOCOLS                      ,   "OPT_TLS_PROTOCOLS"                 },
	{OPT_TLS_CIPHER_SUITE                   ,   "OPT_TLS_CIPHER_SUITE"              },
	{OPT_TLS_CRL_CHECK                      ,   "OPT_TLS_CRL_CHECK"                 },
	{OPT_TLS_CRL_CHECK_ALL                  ,   "OPT_TLS_CRL_CHECK_ALL"             },
	{OPT_TLS_CERT_BLACKLIST                 ,   "OPT_TLS_CERT_BLACKLIST"            },
	{OPT_TLS_LOG_SESSION_INFO               ,   "OPT_TLS_LOG_SESSION_INFO"          },
	{OPT_TLS_KEYFILE                        ,   "OPT_TLS_KEYFILE"                   },
	{OPT_TLS_CERTFILE                       ,   "OPT_TLS_CERTFILE"                  },
	{OPT_READ_DEFAULT_POL                   ,   "OPT_READ_DEFAULT_POL"              },
	{OPT_WRITE_DEFAULT_POL                  ,   "OPT_WRITE_DEFAULT_POL"             },
	{OPT_REMOVE_DEFAULT_POL                 ,   "OPT_REMOVE_DEFAULT_POL"            },
	{OPT_BATCH_DEFAULT_POL                  ,   "OPT_BATCH_DEFAULT_POL"             },
	{OPT_OPERATE_DEFAULT_POL                ,   "OPT_OPERATE_DEFAULT_POL"           },
	{OPT_QUERY_DEFAULT_POL                  ,   "OPT_QUERY_DEFAULT_POL"             },
	{OPT_SCAN_DEFAULT_POL                   ,   "OPT_SCAN_DEFAULT_POL"              },
	{OPT_APPLY_DEFAULT_POL                  ,   "OPT_APPLY_DEFAULT_POL"             },
	{OPT_QUERY_NOBINS                       ,   "OPT_QUERY_NOBINS"                  }
};

static AerospikeStrOptionConstant aerospike_str_option_constants[] = {
	{PHP_POLICY_OPT_DESERIALIZE, "OPT_DESERIALIZE"},
	{PHP_POLICY_OPT_SLEEP_BETWEEN_RETRIES, "OPT_SLEEP_BETWEEN_RETRIES"}
};

bool register_aerospike_option_constants(zend_class_entry* aerospike_ce) {
	AerospikeOptionConstant current_constant;

	for (int i = 0; i < AEROSPIKE_OPTION_CONSTANTS_ARR_SIZE; i++) {
		current_constant = aerospike_option_constants[i];
		zend_declare_class_constant_long(
			aerospike_ce,
			current_constant.constant_str,
			strlen(current_constant.constant_str),
			current_constant.constant_value
		);
	}

	for(int i = 0; i < AEROSPIKE_OPTION_STRCONSTANTS_ARR_SIZE; i++) {
		AerospikeStrOptionConstant current_string_constant = aerospike_str_option_constants[i];
		zend_declare_class_constant_string(
			aerospike_ce,
			current_string_constant.constant_name,
			strlen(current_string_constant.constant_name),
			current_string_constant.constant_str_value
		);
	}

	return true;
}
