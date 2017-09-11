--TEST--
 Basic existsMany operation with empty key array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyEmptyKeyArrayNegative");
--EXPECT--
OK
