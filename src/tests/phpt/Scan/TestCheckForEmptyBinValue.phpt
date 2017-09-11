--TEST--
Scan - Check for empty bin value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckForEmptyBinValue");
--EXPECT--
ERR_PARAM

