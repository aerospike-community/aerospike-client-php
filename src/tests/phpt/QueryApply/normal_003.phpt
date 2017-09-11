--TEST--
queryApply() with job_id(out parameter) is passed of type string

--DESCRIPTION--
This testscase will test for queryApply() with job_id which is output parameter
is passed of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "normal_003");
?>

--EXPECT--
OK
