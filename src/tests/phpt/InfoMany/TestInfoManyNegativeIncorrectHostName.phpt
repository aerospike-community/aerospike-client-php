--TEST--
InfoMany - Negative with incorrect host name

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InfoMany", "testInfoManyNegativeIncorrectHostName");
--EXPECT--
OK
