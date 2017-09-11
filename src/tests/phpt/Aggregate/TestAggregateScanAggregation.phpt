--TEST--
Aggregate - Scan aggregation (empty array given as predicate)

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testScanAggregation");
--EXPECT--
OK

