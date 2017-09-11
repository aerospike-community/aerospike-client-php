--TEST--
Drop role - drop role null

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("DropRole", "testDropRoleNull");
--EXPECT--
ERR_PARAM
