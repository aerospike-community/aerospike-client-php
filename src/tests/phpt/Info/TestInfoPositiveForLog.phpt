--TEST--
Info - Positive for log with response initialized to null

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForLog");
--EXPECT--
OK
