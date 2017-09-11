--TEST--
Scan - Callback Parameter missing

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckForCallbackParameterMissing");
--EXPECT--
ERR_PARAM

