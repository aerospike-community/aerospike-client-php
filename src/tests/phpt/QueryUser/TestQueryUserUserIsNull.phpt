--TEST--
QueryUser - query user is null

--SKIPIF--
<?php include('skip_enterprise.inc'); ?>
--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("QueryUser", "testQueryUserUserIsNull");
--EXPECT--
ERR_PARAM
