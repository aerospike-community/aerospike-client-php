--TEST--
Put - Check List insert in bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testCheckListInsert");
--EXPECT--
OK
