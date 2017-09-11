--TEST--
listRemoveRange() with non-existent index

--DESCRIPTION--
This testcase will test for listRemoveRange() with non-existent index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListRemoveRange", "error_003");
--EXPECT--
ERR_REQUEST_INVALID
