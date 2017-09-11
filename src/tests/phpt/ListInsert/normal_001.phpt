--TEST--
listInsert() with integer value

--DESCRIPTION--
This testcase will add integer at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_001");
--EXPECT--
OK
