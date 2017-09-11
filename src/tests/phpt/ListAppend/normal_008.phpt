--TEST--
listAppend() append empty list to a list

--DESCRIPTION--
This testcase will append empty list to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_008");
--EXPECT--
OK
