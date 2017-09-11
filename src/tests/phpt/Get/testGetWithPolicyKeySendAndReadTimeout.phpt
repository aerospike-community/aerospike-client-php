--TEST--
 Get a record from DB, POLICY_KEY_SEND and read timeout is passed in options. 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetWithPolicyKeySendAndReadTimeout");
--EXPECT--
OK
