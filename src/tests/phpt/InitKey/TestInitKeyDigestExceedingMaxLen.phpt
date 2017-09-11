--TEST--
InitKey - Basic operation with Digest length exceeding max allowable length

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InitKey", "testInitKeyDigestExceedingMaxLen");
--EXPECT--
ERR_CLIENT
