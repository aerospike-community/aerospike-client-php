--TEST--
Scaninfo - info parameter is string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanInfo", "testScanInfoInfoIsString");
--EXPECT--
OK
