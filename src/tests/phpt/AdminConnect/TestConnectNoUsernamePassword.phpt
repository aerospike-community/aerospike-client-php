--TEST--
Admin connect - admin connect no username or password

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("AdminConnect", "testConnectNoUsernamePassword");
--EXPECTREGEX--
(ERR_NOT_AUTHENTICATED|ERR_CLIENT)
