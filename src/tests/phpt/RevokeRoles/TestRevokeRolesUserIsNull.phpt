--TEST--
RevokeRoles - revoke roles with user null

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("RevokeRoles", "testRevokeRolesUserIsNull");
--EXPECT--
ERR_PARAM
