--TEST--
InfoMany - Positive check for build

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyPositiveForBuild");
--EXPECT--
OK
