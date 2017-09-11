--TEST--
operateOrdered() with an integer value is passed in place of an operations array.

--DESCRIPTION--
This testcase will test for operateOrdered() with an integer value is
passed in place of an operations array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "edge_002");
--EXPECT--
ERR_PARAM
