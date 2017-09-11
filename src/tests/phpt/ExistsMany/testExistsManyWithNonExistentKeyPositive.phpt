--TEST--
 Basic existsMany operation with all arguments with one non-existent key.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyWithNonExistentKeyPositive");
--EXPECT--
OK
