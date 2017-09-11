--TEST--
PredicateEquals - predicateEquals with integer value
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Predicate", "testPredicateEqualsValueInt");
--EXPECT--
OK
