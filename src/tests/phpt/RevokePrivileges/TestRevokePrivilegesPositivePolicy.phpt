--TEST--
Revoke Privileges - revoke privileges positive policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("RevokePrivileges", "testRevokePrivilegesPositivePolicy");
--EXPECT--
OK
