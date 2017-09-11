--TEST--
Append with generation policy POLICY_GEN_GT and generation value.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Append", "testAppendWithGenGTPositive");
--EXPECT--
OK

