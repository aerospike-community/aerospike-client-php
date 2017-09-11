--TEST--
ScanApply - Less arguments in udf function definition

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyLuaLessParameter");
--EXPECT--
OK
