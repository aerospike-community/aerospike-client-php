--TEST--
Create user - create user with roles as NULL

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("CreateUser", "testCreateUserRolesNull");
--EXPECT--
ERR_PARAM
