--TEST--
Create user - create user with under priviliged user

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("CreateUser", "testCreateUserUnderPrivileged");
--EXPECT--
ERR_ROLE_VIOLATION
