--TEST--
listInsert() Insert value on  non-existent key

--DESCRIPTION--
This testcase will Insert value on non-existent key.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_010");
--EXPECT--
OK
