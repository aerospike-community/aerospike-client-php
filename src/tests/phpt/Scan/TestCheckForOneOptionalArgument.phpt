--TEST--
Scan - Check for one optional argument

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckForOneOptionalArgument");
--EXPECT--
OK

