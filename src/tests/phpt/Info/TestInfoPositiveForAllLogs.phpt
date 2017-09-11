--TEST--
Info - Positive for all logs

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForAllLogs");
--EXPECT--
OK
