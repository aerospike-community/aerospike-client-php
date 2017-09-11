--TEST--
listInsert() with Insert list 

--DESCRIPTION--
This testcase will add list at the specified index of a list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "normal_006");
--EXPECT--
OK
