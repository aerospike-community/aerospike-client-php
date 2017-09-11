--TEST--
AerospikeSession - Check for session write.

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("AerospikeSession", "testSessionAWrite");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("AerospikeSession", "testSessionAWrite");
--EXPECT--
OK

