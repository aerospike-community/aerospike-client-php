--TEST--
listSize() with all positive arguments

--DESCRIPTION--
This testcase will get count of elements in the list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSize", "normal_001");
--EXPECT--
OK
