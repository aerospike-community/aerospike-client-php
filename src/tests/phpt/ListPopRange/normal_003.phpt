--TEST--
listPopRange() with element i.e. output parameter is passed with string value

--DESCRIPTION--
This testcase will remove the count number of elements from the list
starting at the given index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListPopRange", "normal_003");
--EXPECT--
OK
