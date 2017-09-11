--TEST--
ScanApply - Extra parameter in call to udf

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyExtraParameter");
--EXPECT--
OK
