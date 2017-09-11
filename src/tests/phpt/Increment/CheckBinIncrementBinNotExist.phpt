--TEST--
Increment - bin value by offset bin not exist

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testBinIncrementBinNotExist");
--EXPECT--
OK
