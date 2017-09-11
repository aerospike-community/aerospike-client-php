--TEST--
 PUT List of objects and serializer option is SERIALIZER_NONE.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutListOfObjectsSerializerNone");
--EXPECT--
ERR_PARAM
