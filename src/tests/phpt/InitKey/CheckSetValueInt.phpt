--TEST--
InitKey - Set value integer

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InitKey", "testSetValueInt");
--EXPECT--
OK
