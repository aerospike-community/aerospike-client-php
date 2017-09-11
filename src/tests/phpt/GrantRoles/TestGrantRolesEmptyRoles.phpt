--TEST--
GrantRoles - grant roles is empty

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantRoles", "testGrantRolesEmptyRoles");
--EXPECT--
ERR_PARAM
