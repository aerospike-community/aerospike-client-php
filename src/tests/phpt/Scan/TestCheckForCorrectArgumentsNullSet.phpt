--TEST--
Scan - Check with correct arguments and a NULL set

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testScanWithCorrectArgumentsAndSetNull");
--EXPECT--
OK

