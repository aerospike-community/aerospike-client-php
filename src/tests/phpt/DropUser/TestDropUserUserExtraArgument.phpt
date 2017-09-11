--TEST--
Drop user - drop user extra parameter

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("DropUser", "testDropUserUserExtraArgument");
--EXPECT--
ERR_PARAM
