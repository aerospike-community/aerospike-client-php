--TEST--
ScanApply - Concurrent parameter is positive

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyConcurrentPositive");
--EXPECT--
OK
