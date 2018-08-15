--TEST--
Get - Parameter equence change in Key array

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckParameterSequenceChangeInKeyArray");
--EXPECT--
ERR_CLIENT
