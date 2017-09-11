--TEST--
InitKey - Namespace value integer

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InitKey", "testNameSpaceValueInt");
--EXPECT--
OK
