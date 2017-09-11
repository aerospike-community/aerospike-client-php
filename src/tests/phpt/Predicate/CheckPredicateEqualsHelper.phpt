--TEST--
PredicateEquals - Basic Operation

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Predicate", "testPredicateEqualsHelper");
--EXPECT--
OK
