--TEST--
Operate - Operate with return value as string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateReturnIsStringPositive");
--EXPECT--
OK
