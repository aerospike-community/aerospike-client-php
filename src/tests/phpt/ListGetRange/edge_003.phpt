--TEST--
listGetRange() with index parameter is of type string

--DESCRIPTION--
This testcase will test for index parameter is of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGetRange", "edge_003");
--EXPECT--
ERR_PARAM
