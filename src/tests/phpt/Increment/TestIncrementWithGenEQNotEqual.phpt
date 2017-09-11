--TEST--
Increment with generation policy POLICY_GEN_EQ not equal

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testIncrementWithGenEQNotEqual");
--EXPECT--
OK

