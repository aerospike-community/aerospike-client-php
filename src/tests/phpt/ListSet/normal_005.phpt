--TEST--
listSet() with object

--DESCRIPTION--
This testcase will set object at the specified index of a
list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_005");
--EXPECT--
OK
