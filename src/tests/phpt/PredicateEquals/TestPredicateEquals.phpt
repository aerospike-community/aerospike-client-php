--TEST--
PredicateEquals - CorrectArguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateEquals", "testPredicateEquals");
--EXPECT--
OK

