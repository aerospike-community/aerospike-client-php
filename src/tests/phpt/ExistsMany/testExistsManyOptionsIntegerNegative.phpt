--TEST--
Basic existsMany, Options is of type integer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyOptionsIntegerNegative");
--EXPECT--
ERR_PARAM
