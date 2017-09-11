--TEST--
Scan - Incorrect value for options

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckIncorrectValueForOptions");
--EXPECT--
ERR_PARAM

