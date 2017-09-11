--TEST--
Touch with generation policy POLICY_GEN_EQ positive.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Touch", "testTouchWithGenEQPositive");
--EXPECT--
OK

