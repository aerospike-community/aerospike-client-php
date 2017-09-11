--TEST--
Keyremove with generation policy POLICY_KEY_DIGEST

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testKeyRemoveWithPolicyKeyDigest");
--EXPECT--
ERR_RECORD_NOT_FOUND
