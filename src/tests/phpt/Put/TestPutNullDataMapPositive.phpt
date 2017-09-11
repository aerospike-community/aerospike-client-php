--TEST--
Put NULL data within map (example: ['my_bin' => ['null_value' => NULL]])

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNullDataMapPositive");
--EXPECT--
OK
