--TEST--
GetNodes - Positive test with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetNodes", "testGetNodesWithParameter");
--EXPECT--
OK
