--TEST--
ScanApply - Priority is string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyPriorityIsString");
--EXPECT--
ERR_PARAM
