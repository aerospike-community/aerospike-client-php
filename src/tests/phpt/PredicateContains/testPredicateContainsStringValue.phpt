--TEST--
PredicateBetween - predicate between has array parameter

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateContains", "testPredicateContainsStringValue");
--EXPECT--
OK

