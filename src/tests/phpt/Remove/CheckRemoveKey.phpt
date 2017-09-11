--TEST--
Remove - Remove key from database

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testKeyRemove");
--EXPECT--
ERR_RECORD_NOT_FOUND
