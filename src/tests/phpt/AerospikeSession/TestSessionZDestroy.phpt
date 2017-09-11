--TEST--
AerospikeSession - Destroy the session.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("AerospikeSession", "testSessionZDestroy");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("AerospikeSession", "testSessionZDestroy");
--EXPECT--
ERR_RECORD_NOT_FOUND

