--TEST--
List registered all type(UDF_TYPE_*) modules.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Udf", "testUdfPositiveListRegisteredModules");
--EXPECT--
OK
