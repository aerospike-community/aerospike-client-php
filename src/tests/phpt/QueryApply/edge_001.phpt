--TEST--
queryApply() without any parameter

--DESCRIPTION--
This testscase will test for queryApply() with no arguments.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_001");
?>

--EXPECT--
OK
