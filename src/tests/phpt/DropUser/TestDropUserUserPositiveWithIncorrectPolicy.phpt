--TEST--
Drop user - drop user with incorrect policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("DropUser", "testDropUserUserPositiveWithIncorrectPolicy");
--EXPECT--
ERR_PARAM
