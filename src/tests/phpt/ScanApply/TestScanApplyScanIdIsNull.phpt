--TEST--
ScanApply - Scan Id is null string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyScanIdIsNull");
--EXPECT--
OK
