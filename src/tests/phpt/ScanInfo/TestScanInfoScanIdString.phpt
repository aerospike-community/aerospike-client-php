--TEST--
Scaninfo - scan id is a string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanInfo", "testScanInfoScanIdString");
--EXPECT--
ERR_PARAM

