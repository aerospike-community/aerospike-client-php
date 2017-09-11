--TEST--
Scan - No parmaeters

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Scan", "testScanNoParameter");
--EXPECT--
ERR_PARAM

