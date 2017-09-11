--TEST--
Deregisters UDF module at the Aerospike DB with policy which is present.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfDeregisterModulePositivePolicy");
--EXPECT--
OK
