--TEST--
listInsert() with string value 

--DESCRIPTION--
This testcase will add string at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_002");
--EXPECT--
OK
