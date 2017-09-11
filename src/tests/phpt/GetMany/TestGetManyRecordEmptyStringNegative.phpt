--TEST--
GetMany - with return record parameter as empty string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyRecordEmptyStringNegative");
--EXPECT--
OK

