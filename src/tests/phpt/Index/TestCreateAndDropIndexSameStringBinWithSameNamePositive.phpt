--TEST--
addIndex() and dropIndex()- index with same name on same string bin recreated.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexSameStringBinWithSameNamePositive");
--EXPECT--
ERR_INDEX_FOUND

