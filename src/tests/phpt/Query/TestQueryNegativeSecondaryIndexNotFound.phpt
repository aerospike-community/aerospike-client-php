--TEST--
Query - Negative case with query on non-indexed bin in db

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryNegativeSecondaryIndexNotFound");
--EXPECT--
ERR_INDEX

