--TEST--
Prepend with generation policy POLICY_KEY_SEND.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Prepend", "testPrependWithPolicyKeySend");
--EXPECT--
OK

