--TEST--
GetMany - with no arguments.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyNoArgumentsNegative");
--EXPECT--
OK

