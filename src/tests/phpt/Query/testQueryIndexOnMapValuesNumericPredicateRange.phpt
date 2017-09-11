--TEST--
PredicateContains - Predicate without any parameter.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryIndexOnMapValuesNumericPredicateRange");
--EXPECT--
OK
