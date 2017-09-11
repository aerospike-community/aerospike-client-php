--TEST--
Revoke privileges - revoke privileges empty privilege list

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("RevokePrivileges", "testRevokePrivilegesEmptyPrivilegeList");
--EXPECT--
ERR_PARAM
