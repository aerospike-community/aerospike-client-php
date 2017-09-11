--TEST--
listTrim() with count value is greater than size of the list

--DESCRIPTION--
This testcase will remove all the elements that are not in the range.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListTrim", "normal_002");
--EXPECT--
OK
