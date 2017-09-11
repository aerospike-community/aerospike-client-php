--TEST--
Put - Empty PHP array into a bin

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutEmptyArrayIntoBin");
--EXPECT--
OK
