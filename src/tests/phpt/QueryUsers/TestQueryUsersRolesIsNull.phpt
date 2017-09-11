--TEST--
QueryUsers - query roles is null

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryUsers", "testQueryUsersRolesIsNull");
--EXPECT--
OK
