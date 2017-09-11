--TEST--
Get - GET With Third Parameter not an array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckThirdParameterTypeArray");
--EXPECT--
ERR_PARAM
