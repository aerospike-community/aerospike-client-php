--TEST--
Query - where parameter contains null value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithWhereContainingNullValue");
--EXPECT--
ERR_PARAM
