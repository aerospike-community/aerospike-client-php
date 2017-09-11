--TEST--
Operate - Operate positive TTL
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateWithTTLPositive");
--EXPECT--
OK
