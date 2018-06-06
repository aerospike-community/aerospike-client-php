dnl $Id$
dnl config.m4 for extension aerospike

PHP_ARG_ENABLE(aerospike, whether to enable aerospike support,
[  --enable-aerospike           Enable aerospike support])

if test "$PHP_AEROSPIKE" != "no"; then
  PHP_NEW_EXTENSION(aerospike, 
                    aerospike.c\
                    conversions.c\
                    logging.c\
                    persistent_list.c\
                    register_constants.c\
                    policy_conversions.c\
                    register_policy_constants.c\
                    serializers.c\
                    tls_config.c\
                    user_callbacks.c\
                    client/aerospike_class.c\
                    client/aerospike_session.c\
                    client/admin.c\
                    client/append.c\
                    client/apply.c\
                    client/exists.c\
                    client/exists_many.c\
                    client/get.c\
                    client/get_many.c\
                    client/get_key_digest.c\
                    client/get_nodes.c\
					client/increment.c\
					client/info.c\
					client/job_info.c\
					client/list_operations.c\
					client/log_handlers.c\
					client/operate.c\
					client/php_client_utils.c\
                    client/predicate.c\
                    client/prepend.c\
                    client/put.c\
                    client/query.c\
                    client/remove.c\
                    client/remove_bin.c\
                    client/scan.c\
                    client/scan_apply.c\
                    client/scan_info.c\
                    client/secondary_index.c\
                    client/touch.c\
                    client/truncate.c\
                    client/udf.c\
                    client/user_serializers.c,
                    $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
