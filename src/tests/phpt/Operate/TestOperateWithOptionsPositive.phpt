--TEST--
Operate - Operate positive

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateWithOptionsPositive");
--EXPECT--
OK
