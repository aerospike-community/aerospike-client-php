--TEST--
dropIndex - Non-existing index

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testDropIndexIntegerBinInvalidIndexNegative");
--EXPECT--
ERR_INDEX_NOT_FOUND

