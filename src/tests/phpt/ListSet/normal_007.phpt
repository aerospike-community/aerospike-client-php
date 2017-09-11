--TEST--
listSet() with Set map to the list

--DESCRIPTION--
This testcase will set map at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_007");
--EXPECT--
OK
