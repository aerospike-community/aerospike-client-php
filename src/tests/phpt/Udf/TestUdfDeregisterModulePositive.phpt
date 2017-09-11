--TEST--
Deregisters UDF module at the Aerospike DB which is present.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfDeregisterModulePositive");
--EXPECT--
OK
