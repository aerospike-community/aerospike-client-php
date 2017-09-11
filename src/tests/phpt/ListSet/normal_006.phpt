--TEST--
listSet() with Set list 

--DESCRIPTION--
This testcase will set list at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_006");
--EXPECT--
OK
