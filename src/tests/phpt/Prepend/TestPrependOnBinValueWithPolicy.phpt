--TEST--
Prepend - Basic prepend operation with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Prepend", "testPrependOnBinValueWithPolicy");
--EXPECT--
OK
