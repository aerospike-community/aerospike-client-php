--TEST--
Drop user - drop user is positive with policy

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("DropUser", "testDropUserUserPositiveWithPolicy");
--EXPECT--
OK
