--TEST--
listAppend() with string value 

--DESCRIPTION--
This testcase will add string to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_002");
--EXPECT--
OK
