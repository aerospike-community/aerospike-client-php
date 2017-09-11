--TEST--
queryApply() with bad input predicate is of type integer

--DESCRIPTION--
This testscase will test for queryApply() with where predicate is of type
integer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_003");
?>

--EXPECT--
ERR_PARAM
