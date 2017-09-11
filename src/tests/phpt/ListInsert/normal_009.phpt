--TEST--
listInsert() Insert value on non-existent bin

--DESCRIPTION--
This testcase will Insert value on non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_009");
--EXPECT--
OK
