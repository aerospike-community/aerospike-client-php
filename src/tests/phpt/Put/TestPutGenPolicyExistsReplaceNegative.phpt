--TEST--
PUT with generation policy POLICY_EXISTS_REPLACE negative.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyExistsReplaceNegative");
--EXPECT--
ERR_RECORD_NOT_FOUND
