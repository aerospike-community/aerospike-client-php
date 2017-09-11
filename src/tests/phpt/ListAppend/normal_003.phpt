--TEST--
listAppend() with float value

--DESCRIPTION--
This testcase will add float to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_003");
--EXPECT--
OK
