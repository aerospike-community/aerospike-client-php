--TEST--
Query - Wrong bin name in predicate

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithWrongBinNameInWhere");
--EXPECT--
ERR_INDEX
