--TEST--
Admin connect - admin connect positive

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("AdminConnect", "testConnectPositive");
--EXPECT--
OK
