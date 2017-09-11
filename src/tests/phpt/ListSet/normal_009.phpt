--TEST--
listSet() Set value on non-existent bin

--DESCRIPTION--
This testcase will Set value on non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSet", "normal_009");
--EXPECT--
OK
