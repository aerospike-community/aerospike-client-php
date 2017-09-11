--TEST--
List registered LUA(UDF_TYPE_LUA) modules.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveListRegisteredLUAModules");
--EXPECT--
OK
