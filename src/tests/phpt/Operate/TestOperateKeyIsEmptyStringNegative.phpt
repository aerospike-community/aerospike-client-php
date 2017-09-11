--TEST--
Operate - Operate with key as empty string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "testOperateKeyIsEmptyStringNegative");
--EXPECT--
ERR_PARAM
