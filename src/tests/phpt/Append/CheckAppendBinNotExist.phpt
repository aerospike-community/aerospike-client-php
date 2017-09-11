--TEST--
Append - Basic append operation bin not exists.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Append", "testAppendBinNotExist");
--EXPECT--
OK
