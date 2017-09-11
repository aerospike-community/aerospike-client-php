--TEST--
Get - Get float value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "normal_001");
--EXPECT--
OK
