--TEST--
Apply UDF with parameter NULL

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveApplyParameterNull");
--EXPECT--
OK
