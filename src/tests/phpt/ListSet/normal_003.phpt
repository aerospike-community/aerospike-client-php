--TEST--
listSet() with float value

--DESCRIPTION--
This testcase will set float at the specified index of a
list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_003");
--EXPECT--
OK
