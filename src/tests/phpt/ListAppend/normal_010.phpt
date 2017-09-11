--TEST--
listAppend() append value on  non-existent key

--DESCRIPTION--
This testcase will append value on non-existent key.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_010");
--EXPECT--
OK
