--TEST--
Put - Multi PUT operation

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testMultiPUT");
--EXPECT--
OK
