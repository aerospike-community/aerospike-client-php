--TEST--
Info - Positive for config with response as array

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForConfig");
--EXPECT--
OK
