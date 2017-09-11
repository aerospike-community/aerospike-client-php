--TEST--
Scaninfo - With optionals

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanInfo", "testScanInfoWithOptionals");
--EXPECT--
OK

