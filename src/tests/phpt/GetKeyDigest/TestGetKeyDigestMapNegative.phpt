--TEST--
GetKeyDigest - Negative getKeyDigest operation with Map Key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetKeyDigest", "testGetKeyDigestMapNegative");
--EXPECT--
ERR_PARAM
