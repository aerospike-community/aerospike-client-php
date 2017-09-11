--TEST--
Put - PUT operation with key as unicode character

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPUTWithKeyAsUnicodeCharacter");
--EXPECT--
OK
