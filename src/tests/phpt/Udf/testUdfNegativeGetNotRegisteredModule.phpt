--TEST--
Get contents of a module which is not registered.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfNegativeGetNotRegisteredModule");
--EXPECT--
ERR_UDF
