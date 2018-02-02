--TEST--
addIndex() and dropIndex()- index with different name on same string bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexSameStringBinWithDifferentNamePositive");
--EXPECT--
ERR_INDEX_FOUND

