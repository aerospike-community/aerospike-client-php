--TEST--
Append - Basic append operation.Appended value not string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Append", "testAppendValueNotString");
--EXPECT--
OK
