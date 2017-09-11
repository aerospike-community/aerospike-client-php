--TEST--
addIndex() and dropIndex()- correct arguments for string index

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexStringBinPositive");
--EXPECT--
OK

