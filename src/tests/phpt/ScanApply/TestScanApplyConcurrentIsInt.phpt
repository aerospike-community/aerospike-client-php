--TEST--
ScanApply - Concurrent parameter is Integer

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyConcurrentIsInt");
--EXPECT--
ERR_PARAM
