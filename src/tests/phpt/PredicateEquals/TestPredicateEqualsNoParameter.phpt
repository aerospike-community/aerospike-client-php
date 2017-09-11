--TEST--
PredicateEquals - predicate equals has no parameter

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateEquals", "testPredicateEqualsNoParameter");
--EXPECT--
ERR_PARAM

