--TEST--
addIndex() and dropIndex() - correct arguments for integer index

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexIntegerBinPositive");
--EXPECT--
OK

