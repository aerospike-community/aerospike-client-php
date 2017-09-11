--TEST--
listGetRange() with all positive arguments

--DESCRIPTION--
This testcase will get the elements from the list from the given index to
count number of elements.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGetRange", "normal_001");
--EXPECT--
OK
