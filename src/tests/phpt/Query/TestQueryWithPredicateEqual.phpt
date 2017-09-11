--TEST--
Query - query call with correct arguments and predicate equal

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithPredicateEqual");
--EXPECT--
OK

