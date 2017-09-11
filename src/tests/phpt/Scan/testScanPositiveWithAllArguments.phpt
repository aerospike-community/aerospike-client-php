--TEST--
Scan - Check for all positive arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testScanPositiveWithAllArguments");
--EXPECT--
OK

