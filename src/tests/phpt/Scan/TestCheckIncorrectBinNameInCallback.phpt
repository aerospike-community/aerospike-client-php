--TEST--
Scan - Incorrect bin name in callback

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckIncorrectBinNameInCallback");
--EXPECT--
OK

