--TEST--
Remove - Remove bin with options

--SKIPIF--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_skipif("Remove", "testBinRemoveWithOptions");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "testBinRemoveWithOptions");
--EXPECT--
OK
