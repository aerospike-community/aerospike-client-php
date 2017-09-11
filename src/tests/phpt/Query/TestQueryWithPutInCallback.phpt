--TEST--
Query - query call with put in callback

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithPutInCallback");
--EXPECT--
OK

