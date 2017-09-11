--TEST--
Put - Boolean value asa a key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutMapOfBoolsKeyIsBool");
--EXPECT--
OK
