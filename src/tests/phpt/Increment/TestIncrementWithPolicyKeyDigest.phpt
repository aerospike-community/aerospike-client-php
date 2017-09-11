--TEST--
Increment with generation policy POLICY_KEY_DIGEST

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testIncrementWithPolicyKeyDigest");
--EXPECT--
OK

