--TEST--
Put - PUT With Int String List

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testIntStringList");
--EXPECT--
OK
