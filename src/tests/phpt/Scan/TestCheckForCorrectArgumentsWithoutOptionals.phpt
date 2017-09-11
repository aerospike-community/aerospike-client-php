--TEST--
Scan - Correct arguments without optionals

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckForCorrectArgumentsWithoutOptionals");
--EXPECT--
OK

