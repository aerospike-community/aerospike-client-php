--TEST--
ChangePassword - change password null user

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ChangePassword", "testChangePasswordNullUser");
--EXPECT--
ERR_PARAM
