--TEST--
Query - query call with optional arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithOptionalArguments");
--EXPECT--
OK

