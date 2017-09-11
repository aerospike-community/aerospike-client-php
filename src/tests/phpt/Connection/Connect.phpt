--TEST--
Connection - Basic connection test to the Aerospike DB

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testConnect");
--EXPECT--
OK
