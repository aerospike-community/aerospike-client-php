--TEST--
Connection - Check persist flag passed as true.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
if (!defined('AEROSPIKE_CONFIG_NAME2') || !defined('AEROSPIKE_CONFIG_PORT2')) {
    die("skip multi-host config tests due to a misconfigured aerospike.inc file");
}
if (!check_for_socket(AEROSPIKE_CONFIG_NAME, AEROSPIKE_CONFIG_PORT)) {
    die("skip multi-host config test, the Aerospike DB is not answering at ". AEROSPIKE_CONFIG_NAME. ':'. AEROSPIKE_CONFIG_PORT);
}
if (!check_for_socket(AEROSPIKE_CONFIG_NAME2, AEROSPIKE_CONFIG_PORT2)) {
    die("skip multi-host config tests, the Aerospike DB is not answering at ". AEROSPIKE_CONFIG_NAME2. ':'. AEROSPIKE_CONFIG_PORT2);
}
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testConnectWithPersistflagTrue");
--EXPECT--
OK
