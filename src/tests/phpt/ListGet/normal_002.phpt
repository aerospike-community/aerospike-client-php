--TEST--
listGet() with index value is negative

--DESCRIPTION--
This testcase will get the element from the list at the given index,
Index value is negative integer so it will get element indexing from end.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGet", "normal_002");
--EXPECT--
OK
