--TEST--
 Basic existsMany operation with empty string passed 
 in place of key array .

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyKeyEmptyStringNegative");
--EXPECT--
ERR_PARAM
