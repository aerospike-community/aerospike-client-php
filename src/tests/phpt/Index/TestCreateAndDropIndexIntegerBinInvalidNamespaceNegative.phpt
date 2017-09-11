--TEST--
addIndex() and dropIndex() - Invalid namespace

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexIntegerBinInvalidNamespaceNegative");
--EXPECT--
ERR_REQUEST_INVALID

