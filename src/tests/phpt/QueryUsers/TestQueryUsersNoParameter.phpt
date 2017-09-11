--TEST--
QueryUsers - query users has no parameter

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryUsers", "testQueryUsersNoParameter");
--EXPECT--
ERR_PARAM

