--TEST--
listSize() with count i.e. output parameter is of type string

--DESCRIPTION--
This testcase will get count of elements in the list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSize", "normal_002");
--EXPECT--
OK
