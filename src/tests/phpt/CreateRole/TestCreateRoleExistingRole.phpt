--TEST--
Create role - create role existing role

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("CreateRole", "testCreateRoleExistingRole");
--EXPECT--
ERR_ROLE_ALREADY_EXISTS
