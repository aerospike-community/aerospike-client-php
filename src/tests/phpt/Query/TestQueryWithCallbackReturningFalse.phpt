--TEST--
Query - callback returning false

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithCallbackReturningFalse");
--EXPECT--
OK
