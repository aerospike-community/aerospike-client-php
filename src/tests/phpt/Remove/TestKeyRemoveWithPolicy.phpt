--TEST--
Remove - Remove key from database with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testKeyRemoveWithPolicy");
--EXPECT--
ERR_RECORD_NOT_FOUND
