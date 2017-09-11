--TEST--
Append - Basic append operation

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Append", "testAppendOnBinValue");
--EXPECT--
OK
