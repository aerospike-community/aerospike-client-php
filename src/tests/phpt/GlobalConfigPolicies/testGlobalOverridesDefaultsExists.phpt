--TEST--
Global config overrides csdk defaults(exists).

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GlobalConfigPolicies", "testGlobalOverridesDefaultsExists");
--EXPECT--
OK
