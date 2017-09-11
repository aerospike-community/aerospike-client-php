--TEST--
InitKey - Basic operation with Digest

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InitKey", "testInitKeyDigest");
--EXPECT--
OK
