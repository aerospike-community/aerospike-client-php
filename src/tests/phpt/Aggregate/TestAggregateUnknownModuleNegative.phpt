--TEST--
Aggregate - unknown modules

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testAggregateUnknownModuleNegative");
--EXPECTREGEX--
(ERR_CLIENT|ERR_UDF)

