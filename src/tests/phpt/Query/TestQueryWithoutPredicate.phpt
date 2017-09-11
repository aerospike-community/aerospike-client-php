--TEST--
Query - query without where predicate.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithoutPredicate");
--EXPECT--
OK

