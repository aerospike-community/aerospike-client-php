--TEST--
Put - No parameter

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPUTNoParameter");
--EXPECT--
ERR_PARAM
