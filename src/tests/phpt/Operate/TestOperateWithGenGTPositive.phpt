--TEST--
Operate with generation policy POLICY_GEN_GT and generation value.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateWithGenGTPositive");
--EXPECT--
OK

