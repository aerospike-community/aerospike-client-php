--TEST--
listGet() with non-existent bin

--DESCRIPTION--
This testcase will test for listGet() with non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGet", "error_002");
--EXPECT--
OK
