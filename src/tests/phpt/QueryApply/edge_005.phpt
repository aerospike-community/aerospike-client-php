--TEST--
queryApply() with bad input UDF name is an integer

--DESCRIPTION--
This testscase will test for queryApply() with UDF name is of type
an integer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_005");
?>

--EXPECT--
ERR_PARAM
