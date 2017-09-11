--TEST--
Scan - Check for empty callback funtion

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckForEmptyCallbackFunction");
--EXPECT--
OK

