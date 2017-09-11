--TEST--
 Basic existsMany operation with all arguments.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyPositive");
--EXPECT--
OK
