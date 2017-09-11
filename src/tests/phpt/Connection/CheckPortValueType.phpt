--TEST--
Connection - Check instantiation with a malformed config array (non-integer "port")
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testPortValueIsString");
--EXPECT--
OK
