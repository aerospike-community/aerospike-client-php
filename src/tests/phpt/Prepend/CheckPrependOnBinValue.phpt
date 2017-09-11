--TEST--
Prepend - Basic prepend operation

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Prepend", "testPrependOnBinValue");
--EXPECT--
OK
