--TEST--
Query - query call has empty ns and set

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithEmptyNsSet");
--EXPECT--
ERR_PARAM

