--TEST--
Aggregate - less parameters in udf

--SKIPIF--
<?php include('skip_zts.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Aggregate", "testAggregateLuaLessParameter");
--EXPECTREGEX--
(OK|ERR_CLIENT)
