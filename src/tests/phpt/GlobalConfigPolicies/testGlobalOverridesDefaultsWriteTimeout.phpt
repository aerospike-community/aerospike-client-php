--TEST--
Global config overrides csdk defaults(timeout).

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GlobalConfigPolicies", "testGlobalOverridesDefaultsWriteTimeout");
--EXPECT--
OK
