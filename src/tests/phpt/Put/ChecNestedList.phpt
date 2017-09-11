--TEST--
Put - Nested List with default options.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testNestedList");
--EXPECT--
OK
