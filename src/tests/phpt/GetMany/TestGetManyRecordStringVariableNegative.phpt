--TEST--
GetMany - with return record parameter as a string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyRecordStringVariableNegative");
--EXPECT--
OK

