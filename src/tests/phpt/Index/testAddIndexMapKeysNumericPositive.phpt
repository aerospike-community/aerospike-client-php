--TEST--
Basic index create on map keys and datatype is numeric.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Index", "testAddIndexMapKeysNumericPositive");
--EXPECT--
OK

