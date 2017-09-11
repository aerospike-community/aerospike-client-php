--TEST--
addIndex() and dropIndex() - Very Long Index Name

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testCreateAndDropIndexIntegerBinVeryLongIndexNameNegativeWithDBCrash");
--EXPECTF--
OK
