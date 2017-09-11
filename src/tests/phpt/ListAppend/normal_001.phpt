--TEST--
listAppend() with integer value

--DESCRIPTION--
This testcase will add integer to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_001");
--EXPECT--
OK
