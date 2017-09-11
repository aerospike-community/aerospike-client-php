--TEST--
Put NULL data within list (example: ['my_list' => [1, 'hello', NULL]])

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNullDataListPositive");
--EXPECT--
OK
