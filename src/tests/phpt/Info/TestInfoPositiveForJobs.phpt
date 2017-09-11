--TEST--
Info - Positive for jobs

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForJobs");
--EXPECT--
OK
