--TEST--
listInsert() with object

--DESCRIPTION--
This testcase will add object at the specified index of a
list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_005");
--EXPECT--
OK
