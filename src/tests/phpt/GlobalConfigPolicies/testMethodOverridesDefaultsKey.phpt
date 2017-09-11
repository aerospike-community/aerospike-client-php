--TEST--
Method overrides csdk defaults(key).

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GlobalConfigPolicies", "testMethodOverridesDefaultsKey");
--EXPECT--
OK
