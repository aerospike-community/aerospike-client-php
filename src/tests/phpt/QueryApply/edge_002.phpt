--TEST--
queryApply() with bad input for namespace and set

--DESCRIPTION--
This testscase will test for queryApply() with bad input namespace and set are
of integer type.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "edge_002");
?>

--EXPECT--
ERR_PARAM
