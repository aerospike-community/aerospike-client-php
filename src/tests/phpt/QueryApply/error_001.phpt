--TEST--
queryApply() with non-existent namespace and set

--DESCRIPTION--
This testscase will test for error ERR_REQUEST_INVALID queryApply() with non-existent namespace and
non-existent set.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "error_001");
?>

--EXPECT--
ERR_NAMESPACE_NOT_FOUND
