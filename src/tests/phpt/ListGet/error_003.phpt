--TEST--
listGet() with non-existent index

--DESCRIPTION--
This testcase will test for listGet() with non-existent index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListGet", "error_003");
--EXPECT--
ERR_OP_NOT_APPLICABLE
