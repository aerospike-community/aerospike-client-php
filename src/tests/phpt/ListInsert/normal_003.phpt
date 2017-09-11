--TEST--
listInsert() with float value

--DESCRIPTION--
This testcase will add float at the specified index of a
list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_003");
--EXPECT--
OK
