--TEST--
addIndex() and dropIndex()- index with different name on same integer bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexSameIntegerBinWithDifferentNamePositive");
--EXPECT--
ERR_INDEX_FOUND

