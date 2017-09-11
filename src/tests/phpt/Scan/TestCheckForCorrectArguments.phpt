--TEST--
Scan - Check for correct arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckForCorrectArguments");
--EXPECT--
OK

