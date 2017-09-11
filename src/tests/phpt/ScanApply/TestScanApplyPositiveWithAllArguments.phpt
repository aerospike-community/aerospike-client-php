--TEST--
ScanApply - Correct arguments with optionals

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyPositiveWithAllArguments");
--EXPECT--
OK

