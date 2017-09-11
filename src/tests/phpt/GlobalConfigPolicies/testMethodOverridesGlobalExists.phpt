--TEST--
Method overrides global config (exists).

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GlobalConfigPolicies", "testMethodOverridesGlobalExists");
--EXPECT--
ERR_RECORD_NOT_FOUND
