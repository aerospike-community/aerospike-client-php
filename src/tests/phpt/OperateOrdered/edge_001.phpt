--TEST--
operateOrdered() with string value is passed in place of an keys array.

--DESCRIPTION--
This testcase will test for operateOrdered() with an string value is
passed in place of an keys array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "edge_001");
--EXPECT--
ERR_PARAM
