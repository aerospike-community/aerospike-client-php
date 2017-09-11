--TEST--
Query - query call has wrong bin name in callback

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Query", "testQueryWithWrongBinNameInCallback");
--EXPECT--
ERR_RECORD_NOT_FOUND
