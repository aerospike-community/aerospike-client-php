--TEST--
Get List containing Map of objects with PHP Serializer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetListMapObjectsWithPHPSerializer");
--EXPECT--
OK
