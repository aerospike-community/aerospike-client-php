--TEST--
SetPassword - set password incorrect timeout

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("SetPassword", "testSetPasswordIncorrectTimeout");
--EXPECT--
ERR_PARAM
