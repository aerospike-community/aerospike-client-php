--TEST--
operateOrdered() with OPERATOR_READ on non-existent bin.

--DESCRIPTION--
This testcase will test for operateOrdered() with OPERATOR_READ on non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "normal_004");
?>

--EXPECT--
OK
