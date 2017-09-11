--TEST--
listInsertItems() with items is an empty array

--DESCRIPTION--
This testcase will add items at the specified index of the list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsertItems", "normal_003");
--EXPECT--
OK
