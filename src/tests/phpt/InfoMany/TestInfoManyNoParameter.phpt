--TEST--
InfoMany - Check for no arguments

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyNoParameter");
--EXPECT--
ERR_PARAM
