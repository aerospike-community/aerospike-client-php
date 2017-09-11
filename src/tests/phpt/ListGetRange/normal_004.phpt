--TEST--
listGetRange() with the non-existent bin

--DESCRIPTION--
This testcase will return an empty array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGetRange", "normal_004");
--EXPECT--
OK
