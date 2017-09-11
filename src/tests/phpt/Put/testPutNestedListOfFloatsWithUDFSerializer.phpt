--TEST--
PUT Nested List of floats with UDF serializer. 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNestedListOfFloatsWithUDFSerializer");
--EXPECT--
OK
