--TEST--
queryApply() with bad input LUA arguments are not passed as an array

--DESCRIPTION--
This testscase will test for queryApply() with LUA arguments are paseed
as an integer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_006");
?>

--EXPECT--
ERR_PARAM
