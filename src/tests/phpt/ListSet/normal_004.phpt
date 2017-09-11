--TEST--
listSet() with boolean value

--DESCRIPTION--
This testcase will set boolean at the specified index of a
list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_004");
--EXPECT--
OK
