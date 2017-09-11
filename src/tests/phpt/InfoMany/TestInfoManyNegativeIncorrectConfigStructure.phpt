--TEST--
InfoMany - Negative with incorrect structure for config

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyNegativeIncorrectConfigStructure");
--EXPECT--
ERR_PARAM
