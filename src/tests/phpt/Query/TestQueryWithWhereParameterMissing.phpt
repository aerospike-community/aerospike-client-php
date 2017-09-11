--TEST--
Query - Query call with set parameter missing

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithWhereParameterMissing");
--EXPECT--
ERR_PARAM

