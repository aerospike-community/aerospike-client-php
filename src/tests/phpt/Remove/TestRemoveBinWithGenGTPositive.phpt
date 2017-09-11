--TEST--
RemoveBin with generation policy POLICY_GEN_GT and generation value.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testRemoveBinWithGenGTPositive");
--EXPECT--
ERR_RECORD_NOT_FOUND
