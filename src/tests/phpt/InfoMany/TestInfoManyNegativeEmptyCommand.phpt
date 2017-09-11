--TEST--
InfoMany - Negative with empty command

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyNegativeEmptyCommand");
--EXPECT--
OK
