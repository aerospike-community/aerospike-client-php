--TEST--
Removebin with generation policy POLICY_KEY_DIGEST

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testRemoveBinWithPolicyKeyDigest");
--EXPECT--
ERR_RECORD_NOT_FOUND
