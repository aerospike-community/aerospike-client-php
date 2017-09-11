--TEST--
listMerge() with items is an empty array

--DESCRIPTION--
This testcase will add items to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListMerge", "normal_003");
--EXPECT--
OK
