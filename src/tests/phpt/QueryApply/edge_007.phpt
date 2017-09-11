--TEST--
queryApply() with string is passed in place of options array

--DESCRIPTION--
This testscase will test for queryApply() with options array is not of type an
array, Instead it is given as a string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_007");
?>

--EXPECT--
ERR_PARAM
