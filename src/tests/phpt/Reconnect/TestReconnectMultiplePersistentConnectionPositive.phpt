--TEST--
Reconnect - Check for Multiple Reconnects after close for default persistent connection.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("Reconnect", "testReconnectMultiplePersistentConnectionPositive");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Reconnect", "testReconnectMultiplePersistentConnectionPositive");
--EXPECT--
OK

