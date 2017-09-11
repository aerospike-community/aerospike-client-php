--TEST--
QueryUser - query user is non-existent

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryUser", "testQueryUserNonexistentUser");
--EXPECT--
ERR_INVALID_USER
