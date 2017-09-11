--TEST--
operateOrdered() with returned(output parameter) contains string

--DESCRIPTION--
This testcase will test for operateOrdered() with returned(output parameter) contains string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "normal_002");
?>

--EXPECT--
OK
