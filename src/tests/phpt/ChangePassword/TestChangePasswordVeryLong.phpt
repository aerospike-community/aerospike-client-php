--TEST--
ChangePassword - change password very long

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ChangePassword", "testChangePasswordVeryLong");
--EXPECT--
OK
