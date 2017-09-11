--TEST--
ScanApply - Percent is int

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyPercentIsInt");
--EXPECT--
OK
