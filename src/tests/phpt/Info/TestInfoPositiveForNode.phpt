--TEST--
Info - Positive for info of node

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForNode");
--EXPECT--
OK
