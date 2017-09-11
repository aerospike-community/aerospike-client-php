--TEST--
QueryUsers - query users with incorrect policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryUsers", "testQueryUsersIncorrectPolicy");
--EXPECT--
ERR_PARAM
