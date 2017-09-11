--TEST--
listAppend() with append map to the list

--DESCRIPTION--
This testcase will append map to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_007");
--EXPECT--
OK
