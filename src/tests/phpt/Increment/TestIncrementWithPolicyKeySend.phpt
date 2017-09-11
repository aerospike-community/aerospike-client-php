--TEST--
Increment with generation policy POLICY_KEY_SEND.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testIncrementWithPolicyKeySend");
--EXPECT--
OK

