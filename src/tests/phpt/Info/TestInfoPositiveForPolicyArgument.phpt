--TEST--
Info - Positive for optional policy argument

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForPolicyArgument");
--EXPECT--
OK
