--TEST--
listInsert() with boolean value

--DESCRIPTION--
This testcase will add boolean at the specified index of a
list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_004");
--EXPECT--
OK
