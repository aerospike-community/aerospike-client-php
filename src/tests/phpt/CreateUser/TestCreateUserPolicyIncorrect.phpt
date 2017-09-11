--TEST--
Create user - create user policy incorrect

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("CreateUser", "testCreateUserPolicyIncorrect");
--EXPECT--
ERR_PARAM
