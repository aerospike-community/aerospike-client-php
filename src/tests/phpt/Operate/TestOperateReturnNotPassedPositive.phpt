--TEST--
Operate - Operate when return value passed is NULL

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateReturnNotPassedPositive");
--EXPECT--
OK
