--TEST--
PUT Nested List of bools with UDF serializer.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNestedListOfBoolsWithUDFSerializer");
--EXPECT--
OK
