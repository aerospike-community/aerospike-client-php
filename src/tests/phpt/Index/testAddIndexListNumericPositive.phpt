--TEST--
Add Index on a list and datatype is numeric.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testAddIndexListNumericPositive");
--EXPECT--
OK

