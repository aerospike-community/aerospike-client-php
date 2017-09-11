--TEST--
Put - Serialization with mixed types and no explicit serializer policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testMixedTypesWithDefaultSerializer");
--EXPECT--
OK
