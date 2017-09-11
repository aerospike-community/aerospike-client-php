--TEST--
Increment - bin value by offset key not exist

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testBinIncrementKeyNotExist");
--EXPECT--
OK
