--TEST--
Get - GET With Three Parameter

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGETThreeParameter");
--EXPECT--
OK
