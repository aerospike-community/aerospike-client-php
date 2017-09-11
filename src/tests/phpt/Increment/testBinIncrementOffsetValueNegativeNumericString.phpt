--TEST--
Increment - bin value by Negative Numeric String

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Increment",
        "testBinIncrementOffsetValueNegativeNumericString");
--EXPECT--
OK
