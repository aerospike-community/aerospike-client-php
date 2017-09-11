--TEST--
Increment - bin value by offset with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testBinIncrementByOffsetValueWithPolicy");
--EXPECT--
OK
