--TEST--
PredicateBetween - predicate between with correct arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateBetween", "testPredicateBetween");
--EXPECT--
OK

