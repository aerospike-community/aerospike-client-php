--TEST--
 Basic existsMany operation with all arguments and incorrect key at end.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyPositiveIncorrectKey");
--EXPECT--
OK
