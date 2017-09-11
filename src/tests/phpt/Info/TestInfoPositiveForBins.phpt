--TEST--
Info - Positive for bins

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForBins");
--EXPECT--
OK
