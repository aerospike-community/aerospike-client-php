--TEST--
listAppend() with boolean value

--DESCRIPTION--
This testcase will add boolean to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_004");
--EXPECT--
OK
