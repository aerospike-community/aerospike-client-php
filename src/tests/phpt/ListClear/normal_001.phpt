--TEST--
listClear() with all positive arguments

--DESCRIPTION--
This testcase will remove all the elements from the list value in the bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListClear", "normal_001");
--EXPECT--
OK
