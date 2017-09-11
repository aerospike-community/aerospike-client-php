--TEST--
Increment - bin when offset value string

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment", "testBinIncrementOffsetValueString");
--EXPECT--
ERR_PARAM
