--TEST--
Get - GET With No parameter.Expect atleast 2 get 0.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGETNoParameter");
--EXPECT--
ERR_PARAM
