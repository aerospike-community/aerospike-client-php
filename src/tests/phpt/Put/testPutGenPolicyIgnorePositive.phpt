--TEST--
PUT with generation policy POLICY_GEN_IGNORE and no generation value. 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutGenPolicyIgnorePositive");
--EXPECT--
OK
