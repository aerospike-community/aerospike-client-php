--TEST--
PUT Map of objects with Serializer option is SERIALIZER_NONE.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutMapOfObjectsSerializerNone");
--EXPECT--
ERR_PARAM
