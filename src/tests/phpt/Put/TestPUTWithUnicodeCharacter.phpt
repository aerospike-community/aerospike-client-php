--TEST--
Put - PUT operation with unicode

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPUTWithUnicodeCharacter");
--EXPECT--
OK
