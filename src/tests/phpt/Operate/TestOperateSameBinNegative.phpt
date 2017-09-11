--TEST--
Operate - Operate with same bin negative

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateSameBinNegative");
--EXPECT--
OK
