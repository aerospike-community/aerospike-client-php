--TEST--
listSet() Set empty list to a list

--DESCRIPTION--
This testcase will Set empty list at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_008");
--EXPECT--
OK
