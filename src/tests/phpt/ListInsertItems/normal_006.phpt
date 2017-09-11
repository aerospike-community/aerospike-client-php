--TEST--
listInsertItems() with index value is greater than size of the list

--DESCRIPTION--
This testcase will add items at the specified index of the list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsertItems", "normal_006");
--EXPECT--
OK
