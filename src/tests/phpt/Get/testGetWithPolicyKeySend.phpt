--TEST--
Get a record from DB, POLICY_KEY_SEND is passed as a option. 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetWithPolicyKeySend");
--EXPECT--
OK
