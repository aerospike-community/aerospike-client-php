--TEST--
Query role - query role incorrect role name

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryRole", "testQueryRoleIncorrectRoleName");
--EXPECT--
ERR_INVALID_ROLE
