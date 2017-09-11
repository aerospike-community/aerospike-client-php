--TEST--
ScanApply - ScanApply float value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "normal_001");
--EXPECT--
OK
