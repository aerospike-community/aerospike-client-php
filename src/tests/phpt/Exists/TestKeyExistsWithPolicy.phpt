--TEST--
Exists - Get Key metadata if Key exists with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Exists", "testKeyExistsWithPolicy");
--EXPECT--
OK
