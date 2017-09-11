--TEST--
GetKeyDigest - Basic operation with Integer Key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetKeyDigest", "testGetKeyDigestIntegerPositive");
--EXPECT--
OK
