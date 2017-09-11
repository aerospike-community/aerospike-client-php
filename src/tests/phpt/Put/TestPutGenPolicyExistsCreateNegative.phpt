--TEST--
PUT with generation policy POLICY_EXISTS_CREATE negative

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyExistsCreateNegative");
--EXPECT--
ERR_RECORD_EXISTS
