--TEST--
PredicateBetween - predicate between has one argument

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateBetween", "testPredicateBetweenValueHasOneArgument");
--EXPECT--
ERR_PARAM

