--TEST--
Scan - Set and namespance value null

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testCheckSetAndNsValueNull");
--EXPECT--
ERR_PARAM

