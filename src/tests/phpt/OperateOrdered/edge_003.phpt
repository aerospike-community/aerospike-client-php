--TEST--
operateOrdered() with booleanvalue is passed in place of an options array.

--DESCRIPTION--
This testcase will test for operateOrdered() with boolean value is
passed in place of an options array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "edge_003");
--EXPECT--
ERR_PARAM
