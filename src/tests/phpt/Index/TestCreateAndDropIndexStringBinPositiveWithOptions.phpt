--TEST--
addIndex() and dropIndex()- correct arguments for string index with options

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexStringBinWithOptionsPositive");
--EXPECT--
OK

