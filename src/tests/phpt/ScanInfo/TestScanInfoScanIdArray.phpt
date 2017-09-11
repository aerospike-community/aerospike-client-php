--TEST--
Scaninfo - scan id array

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanInfo", "testScanInfoScanIdArray");
--EXPECT--
ERR_PARAM

