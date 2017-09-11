--TEST--
Put - Without key parameter.Only data parameter is available.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPUTWithOnlyDataParameter");
--EXPECT--
ERR_PARAM
