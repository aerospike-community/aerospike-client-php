--TEST--
Get - GET With Wrong number of parameter.Expect atleast 2 get 1.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGETWrongParameter");
--EXPECT--
ERR_PARAM
