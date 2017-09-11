--TEST--
GetMany - with empty key array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyEmptyKeyArrayNegative");
--EXPECT--
OK

