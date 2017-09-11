--TEST--
Grant Privileges - grant privileges existing privilege

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantPrivileges", "testGrantPrivilegesExistingPrivilege");
--EXPECT--
OK
