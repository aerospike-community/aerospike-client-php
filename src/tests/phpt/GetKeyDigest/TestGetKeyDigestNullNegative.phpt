--TEST--
GetKeyDigest - Negative getKeyDigest operation with NULL key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetKeyDigest", "testGetKeyDigestNullNegative");
--EXPECT--
ERR_PARAM
