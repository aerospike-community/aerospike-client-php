--TEST--
ScanApply - Empty module and function

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyEmptyFunctionUDFNameNegative");
--EXPECT--
ERR_PARAM

