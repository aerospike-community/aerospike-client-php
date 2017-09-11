--TEST--
GrantRoles - grant roles has no parameter

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantRoles", "testGrantRolesNoParameter");
--EXPECT--
ERR_PARAM
