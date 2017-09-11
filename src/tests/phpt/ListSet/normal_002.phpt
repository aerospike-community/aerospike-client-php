--TEST--
listSet() with string value 

--DESCRIPTION--
This testcase will set string at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_002");
--EXPECT--
OK
