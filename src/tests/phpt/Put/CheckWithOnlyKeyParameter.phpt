--TEST--
Put - Only key parameter.Data parameter missing.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPUTWithOnlyKeyParameter");
--EXPECT--
ERR_PARAM
