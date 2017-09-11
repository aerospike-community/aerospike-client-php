--TEST--
Aggregate - empty function udf name

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testAggregateEmptyFunctionUDFNameNegative");
--EXPECT--
ERR_PARAM
