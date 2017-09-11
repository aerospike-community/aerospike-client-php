--TEST--
Put - PUT List on Wrong IP

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testDisconnectedPut");
--EXPECTREGEX--
(ERR_SERVER|ERR_CLUSTER|ERR_CLIENT)
