--TEST--
QueryUsers - query users uses extra parameter

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryUsers", "testQueryUsersExtraParameter");
--EXPECT--
ERR_PARAM
