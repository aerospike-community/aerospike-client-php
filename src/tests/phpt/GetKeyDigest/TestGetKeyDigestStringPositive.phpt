--TEST--
GetKeyDigest - Basic operation with String Key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetKeyDigest", "testGetKeyDigestStringPositive");
--EXPECT--
OK
