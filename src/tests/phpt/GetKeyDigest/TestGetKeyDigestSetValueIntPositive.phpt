--TEST--
GetKeyDigest - Basic operation with namespace value integer

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetKeyDigest", "testGetKeyDigestNameSpaceValueIntPositive");
--EXPECT--
OK
