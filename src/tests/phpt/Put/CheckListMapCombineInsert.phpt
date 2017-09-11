--TEST--
Put - Check List and Map Combine insert in bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testCheckListMapCombineInsert");
--EXPECT--
OK
