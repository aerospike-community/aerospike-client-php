--TEST--
Put NULL data within record (example: ['null_bin' => NULL])

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNullDataRecordPositive");
--EXPECT--
OK
