--TEST--
addIndex() and dropIndex() - Invalid bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexIntegerBinInvalidBinPositive");
--EXPECT--
OK

