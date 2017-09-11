--TEST--
PredicateEquals - predicateEquals with string value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Predicate", "testPredicateEqualsValueString");
--EXPECT--
OK
