--TEST--
operateOrdered() with policy options

--DESCRIPTION--
This testcase will test for operateOrdered() with policy options.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "normal_001");
?>

--EXPECT--
OK
