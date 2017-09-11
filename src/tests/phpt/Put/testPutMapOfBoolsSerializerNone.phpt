--TEST--
PUT Map of bools and serializer option is SERIALIZER_NONE.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutMapOfBoolsSerializerNone");
--EXPECT--
ERR_PARAM
