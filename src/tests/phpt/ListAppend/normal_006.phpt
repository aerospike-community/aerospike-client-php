--TEST--
listAppend() with append list 

--DESCRIPTION--
This testcase will append list to the end of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListAppend", "normal_006");
--EXPECT--
OK
