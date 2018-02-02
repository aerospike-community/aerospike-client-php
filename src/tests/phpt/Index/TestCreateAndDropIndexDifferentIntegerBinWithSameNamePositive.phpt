--TEST--
addIndex() and dropIndex() - index with same name on different integer bin recreated.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexDifferentIntegerBinWithSameNamePositive");
--EXPECT--
ERR_INDEX_FOUND

