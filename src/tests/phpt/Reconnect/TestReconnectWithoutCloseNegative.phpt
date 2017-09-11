--TEST--
Reconnect - Check for Negative Reconnect without close.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("Reconnect", "testReconnectWithoutCloseNegative");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Reconnect", "testReconnectWithoutCloseNegative");
--EXPECT--
ERR_CLIENT

