--TEST--
GrantRoles - grant roles is positive with incorrect policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantRoles", "testGrantRolesPolicyIncorrect");
--EXPECT--
ERR_PARAM
