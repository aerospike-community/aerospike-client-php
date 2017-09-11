--TEST--
listRemove() with all positive arguments

--DESCRIPTION--
This testcase will remove the element from the list at the given index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListRemove", "normal_001");
--EXPECT--
OK
