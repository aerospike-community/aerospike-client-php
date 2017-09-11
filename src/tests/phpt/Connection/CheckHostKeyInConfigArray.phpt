--TEST--
Connection - Check instantiation with a malformed config array (missing "hosts")
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testMissingHostsKeyFromConfigArray");
--EXPECT--
ERR_PARAM
