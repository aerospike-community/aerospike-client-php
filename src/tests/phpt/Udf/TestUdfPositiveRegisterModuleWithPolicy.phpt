--TEST--
Registers UDF Module with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveRegisterModuleWithPolicy");
--EXPECT--
OK
