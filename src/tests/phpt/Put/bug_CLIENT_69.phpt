--TEST--
put() with no data

--DESCRIPTION--
This testcase will test for put() with no bin => val pair given.
(i.e. empty array)

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "bug_CLIENT_69");
?>
--EXPECT--
ERR_PARAM
