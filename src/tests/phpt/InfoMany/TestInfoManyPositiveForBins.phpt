--TEST--
InfoMany - Positive check for bins

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyPositiveForBins");
--EXPECT--
OK
