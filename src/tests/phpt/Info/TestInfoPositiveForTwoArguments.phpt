--TEST--
Info - Positive for 2 arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForTwoArguments");
--EXPECT--
OK
