--TEST--
Connection - Check instantiation with a malformed config array (missing "port")
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testMissingPortKeyFromConfigArray");
--EXPECT--
ERR_PARAM
