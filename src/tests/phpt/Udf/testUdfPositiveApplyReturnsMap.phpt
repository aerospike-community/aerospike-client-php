--TEST--
Apply UDF on a record and UDF returns a Map.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveApplyReturnsMap");
--EXPECT--
OK
