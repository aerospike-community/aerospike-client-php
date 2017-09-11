--TEST--
ScanApply - Arguments to udf are empty

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyArgsEmptyNegative");
--EXPECT--
ERR_PARAM

