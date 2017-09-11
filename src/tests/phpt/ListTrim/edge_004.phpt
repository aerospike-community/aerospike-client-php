--TEST--
listTrim() with count parameter is of type string

--DESCRIPTION--
This testcase will test for count parameter is of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListTrim", "edge_004");
--EXPECT--
ERR_PARAM
