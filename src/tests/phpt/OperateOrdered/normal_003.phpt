--TEST--
operateOrdered() with OP_LIST_POP and OP_LIST_GET on the same bin

--DESCRIPTION--
This testcase will test for operateOrdered() with OP_LIST_POP and OP_LIST_GET on the same bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "normal_003");
?>

--EXPECT--
OK
