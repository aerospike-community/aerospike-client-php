--TEST--
Put - PUT With Associated Array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutAssocArray");
--EXPECT--
OK
