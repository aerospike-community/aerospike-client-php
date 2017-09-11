--TEST--
Get - Check Set Value not exist in Database

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckSetValueNotExistInDB");
--EXPECT--
ERR_RECORD_NOT_FOUND
