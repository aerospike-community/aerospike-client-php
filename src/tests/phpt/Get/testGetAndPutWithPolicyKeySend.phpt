--TEST--
 Get a record from DB, POLICY_KEY_SEND and read timeout is passed in
 options. Also, While Put POLICY_KEY_SEND option is passed. 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetAndPutWithPolicyKeySend");
--EXPECT--
OK
