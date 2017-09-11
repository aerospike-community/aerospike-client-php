--TEST--
Query - query call has incorrect name of bins

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithIncorrectNameOfBins");
--EXPECT--
ERR_RECORD_NOT_FOUND
