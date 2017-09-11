--TEST--
queryApply() with bad input module is of type integer

--DESCRIPTION--
This testscase will test for queryApply() with module name is of type
integer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_004");
?>

--EXPECT--
ERR_PARAM
