--TEST--
Query - query call with empty callback function

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithEmptyCallbackFunction");
--EXPECT--
OK
