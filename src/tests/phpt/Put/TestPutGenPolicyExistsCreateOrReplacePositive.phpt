--TEST--
PUT with generation policy POLICY_EXISTS_CREATE_OR_REPLACE positive.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyExistsCreateOrReplacePositive");
--EXPECT--
OK
