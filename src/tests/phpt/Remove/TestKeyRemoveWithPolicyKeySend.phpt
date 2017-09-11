--TEST--
KeyRemove with generation policy POLICY_KEY_SEND.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testKeyRemoveWithPolicyKeySend");
--EXPECT--
ERR_RECORD_NOT_FOUND
