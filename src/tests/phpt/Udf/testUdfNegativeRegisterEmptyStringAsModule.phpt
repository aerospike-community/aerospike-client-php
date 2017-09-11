--TEST--
Register empty string as a UDF module.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfNegativeRegisterEmptyStringAsModule");
--EXPECT--
ERR_PARAM
