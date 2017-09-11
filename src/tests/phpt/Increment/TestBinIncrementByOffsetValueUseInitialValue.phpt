--TEST--
Increment - bin value by offset use initial value

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testBinIncrementByOffsetValueUseInitialValue");
--EXPECT--
OK
