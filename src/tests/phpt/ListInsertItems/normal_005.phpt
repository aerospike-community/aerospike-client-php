--TEST--
listInsertItems() with non-existent key

--DESCRIPTION--
This testcase will add items at the specified index of the list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsertItems", "normal_005");
--EXPECT--
OK
