--TEST--
InfoMany - Positive check for bins with config and policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyPositiveForBinsWithPolicy");
--EXPECT--
OK
