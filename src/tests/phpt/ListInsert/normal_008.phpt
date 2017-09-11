--TEST--
listInsert() Insert empty list to a list

--DESCRIPTION--
This testcase will Insert empty list at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_008");
--EXPECT--
OK
