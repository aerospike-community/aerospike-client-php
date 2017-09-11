--TEST--
PredicateBetween - predicate between has boolean parameters

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateBetween", "testPredicateBetweenValueIsBool");
--EXPECT--
OK

