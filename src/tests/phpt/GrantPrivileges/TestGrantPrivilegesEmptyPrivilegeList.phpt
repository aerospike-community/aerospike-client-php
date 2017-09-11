--TEST--
Grant privileges - grant privileges empty privilege list

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GrantPrivileges", "testGrantPrivilegesEmptyPrivilegeList");
--EXPECT--
ERR_PARAM
