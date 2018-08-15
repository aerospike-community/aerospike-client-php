--TEST--
Get - Check NameSpace Value not exist in Database

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckNameSpaceValueNotExistInDB");
--EXPECT--
ERR_CLIENT
