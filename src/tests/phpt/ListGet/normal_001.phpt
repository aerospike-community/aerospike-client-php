--TEST--
listGet() with all positive arguments

--DESCRIPTION--
This testcase will get the element from the list at the given index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGet", "normal_001");
--EXPECT--
OK
