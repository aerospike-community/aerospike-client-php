--TEST--
Reconnect - Check for Basic Reconnect after close for default persistent connection.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("Reconnect", "testReconnectPersistentConnectionPositive");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Reconnect", "testReconnectPersistentConnectionPositive");
--EXPECT--
OK

