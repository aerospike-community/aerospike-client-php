--TEST--
listAppend() with object

--DESCRIPTION--
This testcase will add object to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_005");
--EXPECT--
OK
