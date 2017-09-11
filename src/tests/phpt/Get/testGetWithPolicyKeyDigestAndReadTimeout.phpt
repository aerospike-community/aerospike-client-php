--TEST--
 Get a record from DB, POLICY_KEY_DIGEST and read timeout is passed in
 options.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetWithPolicyKeyDigestAndReadTimeout");
--EXPECT--
OK
