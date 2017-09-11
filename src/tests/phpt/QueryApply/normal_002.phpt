--TEST--
queryApply() with all arguments including options

--DESCRIPTION--
This testcase will test for queryApply() with all correct arguments and check
whether records which satisfy predicate are modified through get() API

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryApply", "normal_002");
?>

--EXPECT--
OK
