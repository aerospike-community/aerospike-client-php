--TEST--
listRemoveRange() with options argument is of type string

--DESCRIPTION--
This testcase will test for options argument is of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListRemoveRange", "edge_005");
--EXPECT--
ERR_PARAM
