--TEST--
Connection - Check instantion with config argument that is not an array
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testConfigNotArray");
--EXPECT--
ERR_PARAM
