--TEST--
Aggregate - argument is variable

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testAggregateArgumentIsVariableNegative");
--EXPECT--
ERR_PARAM
