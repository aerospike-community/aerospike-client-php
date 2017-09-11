--TEST--
KeyRemove with generation policy POLICY_GEN_EQ positive.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testKeyRemoveWithGenEQPositive");
--EXPECT--
ERR_RECORD_NOT_FOUND
