--TEST--
RevokeRoles - revoke read role

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("RevokeRoles", "testRevokeRolesReadRole");
--EXPECT--
ERR_ROLE_VIOLATION
