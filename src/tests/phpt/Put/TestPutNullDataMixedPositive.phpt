--TEST--
Put NULL data within mixed nested data.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNullDataMixedPositive");
--EXPECT--
OK
