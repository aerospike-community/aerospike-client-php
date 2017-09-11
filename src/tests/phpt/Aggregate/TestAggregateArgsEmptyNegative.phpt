--TEST--
Aggregate - empty args to udf

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testAggregateArgsEmptyNegative");
--EXPECT--
ERR_PARAM
