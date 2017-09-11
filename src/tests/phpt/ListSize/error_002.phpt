--TEST--
listSize() with non-existent bin

--DESCRIPTION--
This testcase will test for listSize() with non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListSize", "error_002");
--EXPECT--
OK
