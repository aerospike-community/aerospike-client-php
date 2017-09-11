--TEST--
Scan - Incorrect name of bins

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckIncorrectNameOfBins");
--EXPECT--
ERR_CLIENT
