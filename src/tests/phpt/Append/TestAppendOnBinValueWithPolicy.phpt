--TEST--
Append - Basic append operation with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Append", "testAppendOnBinValueWithPolicy");
--EXPECT--
OK
