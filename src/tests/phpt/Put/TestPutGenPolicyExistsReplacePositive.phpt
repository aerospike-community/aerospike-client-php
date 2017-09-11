--TEST--
PUT with generation policy POLICY_EXISTS_REPLACE positive.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyExistsReplacePositive");
--EXPECT--
OK
