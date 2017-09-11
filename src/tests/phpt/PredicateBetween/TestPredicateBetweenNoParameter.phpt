--TEST--
PredicateBetween - predicate between has no parameter

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("PredicateBetween", "testPredicateBetweenNoParameter");
--EXPECT--
ERR_PARAM

