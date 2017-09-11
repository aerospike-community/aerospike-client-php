--TEST--
listGetRange() with index value is negative

--DESCRIPTION--
This testcase will get the element from the list at the given index,
Index value is negative integer so it will get the elements indexing from end.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGetRange", "normal_002");
--EXPECT--
OK
