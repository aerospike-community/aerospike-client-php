--TEST--
Query - query call with empty bin value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithEmptyBinValue");
--EXPECT--
ERR_PARAM
