--TEST--
Get - GET With Second Parameter not an array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckSecondParameterTypeArray");
--EXPECT--
OK
