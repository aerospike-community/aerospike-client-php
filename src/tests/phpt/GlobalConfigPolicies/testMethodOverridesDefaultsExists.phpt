--TEST--
Method overrides csdk defaults(exists).


--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GlobalConfigPolicies", "testMethodOverridesDefaultsExists");
--EXPECT--
ERR_RECORD_NOT_FOUND
