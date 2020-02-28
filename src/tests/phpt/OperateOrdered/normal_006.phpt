--TEST--
operateOrdered() with read and delete operations.

--DESCRIPTION--
This testcase will test for operateOrdered() with OPERATOR_READ and OPERATOR_DELETE on the same key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "normal_006");
?>

--EXPECT--
OK
