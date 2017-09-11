--TEST--
Query role - query role no parameter

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryRole", "testQueryRoleNoParameter");
--EXPECT--
ERR_PARAM
