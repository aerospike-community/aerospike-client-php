--TEST--
PredicateEquals - value of predicate is of type array

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateEquals", "testPredicateEqualsValueIsArray");
--EXPECT--
OK

