--TEST--
ScanApply - Extra parameter in udf function definition

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyLuaExtraParameter");
--EXPECT--
OK
