--TEST--
addIndex() and dropIndex() - Invalid index type, will use string index type by default

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("Index", "testCreateAndDropIndexIntegerBinInvalidIndexTypePositive");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexIntegerBinInvalidIndexTypePositive");
--EXPECT--
OK

