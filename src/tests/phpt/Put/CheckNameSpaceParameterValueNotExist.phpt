--TEST--
Put - Check NameSpace Value not exist in Database

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testCheckNameSpaceValueNotExistInDB");
--EXPECT--
ERR_CLIENT
