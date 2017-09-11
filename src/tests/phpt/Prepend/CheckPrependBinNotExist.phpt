--TEST--
Prepend - Basic prepend operation bin not exists.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Prepend", "testPrependBinNotExist");
--EXPECT--
OK
