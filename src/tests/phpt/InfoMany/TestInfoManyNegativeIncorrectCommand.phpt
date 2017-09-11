--TEST--
InfoMany - Negative with incorrect command

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyNegativeIncorrectCommand");
--EXPECT--
OK
