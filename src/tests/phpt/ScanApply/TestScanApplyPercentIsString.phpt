--TEST--
ScanApply - Percent is string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyPercentIsString");
--EXPECT--
ERR_PARAM
