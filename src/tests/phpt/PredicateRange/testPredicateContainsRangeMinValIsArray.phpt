--TEST--
PredicateBetween - predicate between has array parameter

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateRange", "testPredicateRangeMinValIsArray");
--EXPECT--
ERR_PARAM

