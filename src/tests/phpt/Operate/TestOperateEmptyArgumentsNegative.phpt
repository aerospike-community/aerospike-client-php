--TEST--
Operate - Operate with no arguments


--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateEmptyArgumentsNegative");
--EXPECT--
ERR_CLIENT
