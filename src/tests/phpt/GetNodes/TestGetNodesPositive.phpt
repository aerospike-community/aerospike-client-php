--TEST--
GetNodes - Positive test

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetNodes", "testGetNodesPositive");
--EXPECT--
OK
