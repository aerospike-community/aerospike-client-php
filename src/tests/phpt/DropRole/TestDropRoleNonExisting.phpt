--TEST--
Drop role - drop role not existing

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("DropRole", "testDropRoleNonExisting");
--EXPECT--
ERR_INVALID_ROLE
