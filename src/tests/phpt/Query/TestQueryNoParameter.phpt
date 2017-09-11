--TEST--
Query - query call has no parameters

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryNoParameter");
--EXPECT--
ERR_PARAM

