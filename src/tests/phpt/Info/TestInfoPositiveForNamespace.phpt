--TEST--
Info - Positive for namespace information

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForNamespace");
--EXPECT--
OK
