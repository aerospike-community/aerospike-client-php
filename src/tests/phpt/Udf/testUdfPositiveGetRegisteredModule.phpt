--TEST--
Get registered UDF's code.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveGetRegisteredModule");
--EXPECT--
OK
