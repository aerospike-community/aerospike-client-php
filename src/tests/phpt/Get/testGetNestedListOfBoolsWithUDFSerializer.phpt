--TEST--
Get Nested List of bools with UDF serializer.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetNestedListOfBoolsWithUDFSerializer");
--EXPECT--
OK
