--TEST--
listAppend() append value on non-existent bin

--DESCRIPTION--
This testcase will append value on non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_009");
--EXPECT--
OK
