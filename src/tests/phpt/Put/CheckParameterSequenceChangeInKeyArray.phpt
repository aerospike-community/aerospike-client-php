--TEST--
Put - Parameter equence change in Key array

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testCheckParameterSequenceChangeInKeyArray");
--EXPECT--
ERR_CLIENT
