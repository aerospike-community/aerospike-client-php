--TEST--
Connection - Check instantiation with a null persistence alias

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Connection", "testNullAlias");
--EXPECT--
OK
