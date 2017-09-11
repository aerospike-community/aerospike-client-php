--TEST--
Connection - Check instantiation with a malformed config array (missing "addr")
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testMissingAddrKeyFromConfigArray");
--EXPECT--
ERR_PARAM
