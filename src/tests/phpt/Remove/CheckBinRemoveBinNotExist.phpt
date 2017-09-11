--TEST--
Remove - bin not exist

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testBinRemoveBinNotExist");
--EXPECT--
ERR_REQUEST_INVALID
