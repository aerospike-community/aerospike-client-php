--TEST--
jobInfo() with non-existent module

--DESCRIPTION--
This testscase will test for jobInfo() with non-existent module.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("JobInfo", "error_001");
?>

--EXPECT--
OK
