--TEST--
Tests if all the inserted records are removed by using digest.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testRemoveByDigest");
--EXPECT--
OK
