--TEST--
PUT Map of floats and serializer option is SERIALIZER_NONE.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutMapOfFloatsSerializerNone");
--EXPECT--
OK
