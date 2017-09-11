--TEST--
Create role - create role invalid role type

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("CreateRole", "testCreateRoleInvalidRoleType");
--EXPECT--
ERR_PARAM
