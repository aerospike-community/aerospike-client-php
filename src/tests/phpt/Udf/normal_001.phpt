--TEST--
Udf - Udf appends float value to the existing list.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "normal_001");
--EXPECT--
OK
