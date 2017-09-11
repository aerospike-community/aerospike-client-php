--TEST--
Aggregate - unknown function

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testAggregateUnknownFunctionNegative");
--EXPECTREGEX--
(ERR_CLIENT|ERR_UDF)
