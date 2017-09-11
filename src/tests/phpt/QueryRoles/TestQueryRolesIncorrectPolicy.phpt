--TEST--
Query roles - query all roles incorrect policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryRoles", "testQueryRolesIncorrectPolicy");
--EXPECT--
ERR_PARAM
