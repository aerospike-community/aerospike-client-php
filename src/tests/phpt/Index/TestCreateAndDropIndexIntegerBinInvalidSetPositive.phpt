--TEST--
addIndex() and dropIndex() - Invalid set

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexIntegerBinInvalidSetPositive");
--EXPECT--
OK

