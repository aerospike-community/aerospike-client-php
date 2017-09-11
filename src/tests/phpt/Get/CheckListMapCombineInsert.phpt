--TEST--
Get - Check Linst and Map combine insert in bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckListMapCombineInsert");
--EXPECT--
OK
