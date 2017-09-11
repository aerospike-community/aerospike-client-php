--TEST--
listInsert() with Insert map to the list

--DESCRIPTION--
This testcase will add map at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_007");
--EXPECT--
OK
