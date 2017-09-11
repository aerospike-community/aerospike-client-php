--TEST--
listPopRange() with all positive arguments

--DESCRIPTION--
This testcase will remove the count number of elements from
the list starting at given index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListPopRange", "normal_001");
--EXPECT--
OK
