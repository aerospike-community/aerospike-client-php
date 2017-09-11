#include "register_policy_constants.h"
#include "php_aerospike_types.h"
#include "aerospike/as_status.h"
#include "aerospike/aerospike_index.h"
#include "php_aerospike_types.h"
#include "aerospike/as_scan.h"
#include "aerospike/as_job.h"
#include "aerospike/as_admin.h"

#define AEROSPIKE_OPTION_CONSTANTS_ARR_SIZE (sizeof(aerospike_option_constants)/sizeof(AerospikeOptionConstant))


#define MAX_CONSTANT_STR_SIZE 512

typedef struct _AerospikeOptionConstant {
	long constant_value;
	char constant_str[MAX_CONSTANT_STR_SIZE];
} AerospikeOptionConstant;


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
	{ OPT_SCAN_PERCENTAGE                   ,   "OPT_SCAN_PERCENTAGE"               },
	{ OPT_SCAN_CONCURRENTLY                 ,   "OPT_SCAN_CONCURRENTLY"             },
	{ OPT_SCAN_NOBINS                       ,   "OPT_SCAN_NOBINS"                   },
	{ OPT_SCAN_INCLUDELDT                   ,   "OPT_SCAN_INCLUDELDT"               },
	{ OPT_POLICY_GEN                        ,   "OPT_POLICY_GEN"                    },
	{ OPT_POLICY_REPLICA                    ,   "OPT_POLICY_REPLICA"                },
	{ OPT_POLICY_CONSISTENCY                ,   "OPT_POLICY_CONSISTENCY"            },
	{ OPT_POLICY_COMMIT_LEVEL               ,   "OPT_POLICY_COMMIT_LEVEL"           },
	{ OPT_TTL                               ,   "OPT_TTL"                           },
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
	{ AS_POLICY_CONSISTENCY_LEVEL_ONE       ,   "POLICY_CONSISTENCY_ONE"            },
	{ AS_POLICY_CONSISTENCY_LEVEL_ALL       ,   "POLICY_CONSISTENCY_ALL"            },
	{ AS_POLICY_COMMIT_LEVEL_ALL            ,   "POLICY_COMMIT_LEVEL_ALL"           },
	{ AS_POLICY_COMMIT_LEVEL_MASTER         ,   "POLICY_COMMIT_LEVEL_MASTER"        },
	{ AS_PRIVILEGE_USER_ADMIN               ,   "PRIV_USER_ADMIN"                   },
	{ AS_PRIVILEGE_SYS_ADMIN                ,   "PRIV_SYS_ADMIN"                    },
	{ AS_PRIVILEGE_READ                     ,   "PRIV_READ"                         },
	{ AS_PRIVILEGE_READ_WRITE               ,   "PRIV_READ_WRITE"                   },
	{ AS_PRIVILEGE_READ_WRITE_UDF           ,   "PRIV_READ_WRITE_UDF"               },
	{ AS_PRIVILEGE_DATA_ADMIN               ,   "PRIV_DATA_ADMIN"                   },
	{ OPT_POLICY_DURABLE_DELETE             ,   "OPT_POLICY_DURABLE_DELETE"         }
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
	return true;
}
