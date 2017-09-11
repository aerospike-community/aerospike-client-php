--TEST--
AerospikeSession - Check for session read.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("AerospikeSession", "testSessionBRead");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("AerospikeSession", "testSessionBRead");
--EXPECT--
OK

