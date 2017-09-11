--TEST--
Connection - Check Connect with options.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testConnectWithOption");
--EXPECT--
OK
