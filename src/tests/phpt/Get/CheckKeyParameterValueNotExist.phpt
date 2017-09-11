--TEST--
Get - Check Key Value not exist in Database

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckKeyValueNotExistInDB");
--EXPECT--
ERR_RECORD_NOT_FOUND

