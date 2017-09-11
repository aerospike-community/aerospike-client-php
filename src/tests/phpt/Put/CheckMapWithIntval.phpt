--TEST--
Put - PUT Map with Integer value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testMapWithIntval");
--EXPECT--
OK
