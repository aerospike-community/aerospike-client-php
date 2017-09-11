--TEST--
Query role - query role positive with policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryRole", "testQueryRolePositivePolicy");
--EXPECT--
OK
