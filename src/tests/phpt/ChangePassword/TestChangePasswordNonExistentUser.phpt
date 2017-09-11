--TEST--
ChangePassword - change password non-existent user

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ChangePassword", "testChangePasswordNonExistentUser");
--EXPECT--
ERR_INVALID_USER
