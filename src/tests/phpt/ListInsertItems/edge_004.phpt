--TEST--
listInsertItems() with index argument is of type string

--DESCRIPTION--
This testcase will test for options argument is of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsertItems", "edge_004");
--EXPECT--
ERR_PARAM
