--TEST--
Grant privileges - grant privileges unknown role name

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantPrivileges", "testGrantPrivilegesUnknownRoleName");
--EXPECT--
ERR_INVALID_ROLE
