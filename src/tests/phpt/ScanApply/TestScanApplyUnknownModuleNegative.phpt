--TEST--
ScanApply - Unknown Module

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyUnknownModuleNegative");
--EXPECT--
OK

