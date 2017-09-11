--TEST--
PUT with generation policy POLICY_EXISTS_IGNORE positive.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyExistsIgnorePositive");
--EXPECT--
OK
