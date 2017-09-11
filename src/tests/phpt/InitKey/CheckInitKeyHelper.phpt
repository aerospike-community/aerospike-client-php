--TEST--
InitKey - Basic operation

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("InitKey", "testInitKeyHelper");
--EXPECT--
OK
