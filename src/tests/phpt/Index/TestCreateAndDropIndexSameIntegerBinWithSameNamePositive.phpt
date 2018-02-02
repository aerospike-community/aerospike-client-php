--TEST--
addIndex() and dropIndex()- index with same name on same integer bin recreated.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexSameIntegerBinWithSameNamePositive");
--EXPECT--
ERR_INDEX_FOUND

