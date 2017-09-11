--TEST--
Remove - Remove bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testBinRemove");
--EXPECT--
ERR_RECORD_NOT_FOUND
