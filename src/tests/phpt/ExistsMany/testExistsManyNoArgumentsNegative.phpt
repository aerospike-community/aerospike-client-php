--TEST--
 Basic existsMany operation with no arguments.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyNoArgumentsNegative");
--EXPECT--
ERR_PARAM
