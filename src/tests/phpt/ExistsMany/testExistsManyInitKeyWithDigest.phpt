--TEST--
 Basic existsMany, initKey with digest.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyInitKeyWithDigest");
--EXPECT--
OK

