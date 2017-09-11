--TEST--
listGet() with non-existent key

--DESCRIPTION--
This testcase will test for non-existent key.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGet", "error_001");
--EXPECT--
ERR_RECORD_NOT_FOUND
