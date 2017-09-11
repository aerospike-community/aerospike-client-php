--TEST--
Scan Info - scan id negative

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanInfo", "testScanInfoScanIdNegative");
--EXPECT--
OK

