--TEST--
Get - bin missing in selected bin array

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testCheckBinMissing");
--EXPECT--
ERR_PARAM
