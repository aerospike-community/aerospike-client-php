--TEST--
Apply UDF on a record which returns record.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveApplyReturnsRecord");
--EXPECT--
OK
