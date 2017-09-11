--TEST--
ScanApply - Argument is variable

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyArgumentIsVariableNegative");
--EXPECT--
ERR_PARAM

