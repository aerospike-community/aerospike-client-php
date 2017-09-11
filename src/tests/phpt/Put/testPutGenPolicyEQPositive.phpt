--TEST--
PUT with generation policy POLICY_GEN_EQ and no generation value.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyEQPositive");
--EXPECT--
ERR_RECORD_GENERATION
