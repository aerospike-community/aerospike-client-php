--TEST--
Info - Positive for latency

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForLatency");
--EXPECT--
OK
