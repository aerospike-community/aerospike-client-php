--TEST--
listPop() with element i.e. output parameter is passed with string value

--DESCRIPTION--
This testcase will remove the element from the list at the given index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListPop", "normal_003");
--EXPECT--
OK
