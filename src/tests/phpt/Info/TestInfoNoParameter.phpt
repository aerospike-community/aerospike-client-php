--TEST--
Info - Check for no arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoNoParameter");
--EXPECT--
ERR_PARAM
