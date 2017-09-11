--TEST--
AerospikeSession - Read the session after destroy.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("AerospikeSession", "testSessionZReadAfterDestroy");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("AerospikeSession", "testSessionZReadAfterDestroy");
--EXPECT--
OK

