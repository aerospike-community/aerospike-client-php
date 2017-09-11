--TEST--
Query - query call with correct arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithCorrectArguments");
--EXPECT--
OK

