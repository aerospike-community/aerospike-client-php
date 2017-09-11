--TEST--
PredicateEquals - value is same

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateEquals", "testPredicateEqualsValueIsSame");
--EXPECT--
OK

