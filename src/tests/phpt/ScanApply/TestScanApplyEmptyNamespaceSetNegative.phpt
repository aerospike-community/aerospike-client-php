--TEST--
ScanApply - Unknown namespace and set

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyEmptyNamespaceSetNegative");
--EXPECT--
ERR_PARAM

