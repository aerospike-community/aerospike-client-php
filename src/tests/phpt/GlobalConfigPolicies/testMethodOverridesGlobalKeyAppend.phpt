--TEST--
Append - Method overrides global config(key).

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GlobalConfigPolicies", "testMethodOverridesGlobalKeyAppend");
--EXPECT--
OK
