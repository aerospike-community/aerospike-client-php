--TEST--
ScanApply - Unknown Function

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyUnknownFunctionNegative");
--EXPECT--
OK

