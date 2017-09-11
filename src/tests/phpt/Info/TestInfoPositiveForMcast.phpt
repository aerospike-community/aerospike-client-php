--TEST--
Info - Positive for mcast

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForMcast");
--EXPECT--
OK
