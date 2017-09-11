--TEST--
Revoke privileges - revoke privileges incorrect role type

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("RevokePrivileges", "testRevokePrivilegesIncorrectRoleType");
--EXPECT--
ERR_PARAM
