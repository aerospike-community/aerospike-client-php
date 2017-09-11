--TEST--
jobInfo() with all arguments including options

--DESCRIPTION--
This testcase will test for jobInfo() with all correct arguments and check
for the status of query JOB.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("JobInfo", "normal_002");
?>

--EXPECT--
OK
