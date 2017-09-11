--TEST--
listSet() Set value on  non-existent key

--DESCRIPTION--
This testcase will Set value on non-existent key.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_010");
--EXPECT--
OK
