--TEST--
GetKeyDigest - Negative getKeyDigest operation with List Key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetKeyDigest", "testGetKeyDigestListNegative");
--EXPECT--
ERR_PARAM
