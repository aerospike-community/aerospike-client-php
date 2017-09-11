--TEST--
PredicateEquals - value of predicate is of type boolean

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateEquals", "testPredicateEqualsValueIsBool");
--EXPECT--
OK

