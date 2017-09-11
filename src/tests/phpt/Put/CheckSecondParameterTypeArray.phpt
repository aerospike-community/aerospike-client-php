--TEST--
Put - PUT With Second Parameter not an array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testCheckSecondParameterTypeArray");
--EXPECT--
ERR_PARAM
