--TEST--
jobInfo() with all mandatory arguments

--DESCRIPTION--
This testcase will test for jobInfo() with all correct arguments and check for
status of query JOB.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("JobInfo", "normal_001");
?>

--EXPECT--
OK
