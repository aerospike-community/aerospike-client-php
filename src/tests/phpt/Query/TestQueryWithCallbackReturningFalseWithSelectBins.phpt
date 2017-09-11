--TEST--
Query - callback returning false with select bins in query

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithCallbackReturningFalseWithSelectBins");
--EXPECT--
OK
