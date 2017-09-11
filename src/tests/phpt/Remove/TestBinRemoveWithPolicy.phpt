--TEST--
Remove - Remove bin with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testBinRemoveWithPolicy");
--EXPECT--
ERR_RECORD_NOT_FOUND
