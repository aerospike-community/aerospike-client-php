--TEST--
listGetRange() with non-existent index

--DESCRIPTION--
This testcase will test for listGetRange() with non-existent index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGetRange", "error_002");
--EXPECT--
OK
