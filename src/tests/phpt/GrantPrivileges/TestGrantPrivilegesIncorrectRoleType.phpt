--TEST--
Grant privileges - grant privileges incorrect role type

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantPrivileges", "testGrantPrivilegesIncorrectRoleType");
--EXPECT--
ERR_PARAM
