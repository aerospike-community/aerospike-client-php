--TEST--
Query - query call with incorrect ns and set

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithIncorrectNsSet");
--EXPECT--
ERR_NAMESPACE_NOT_FOUND
