--TEST--
PUT Map containing List of objects with PHP Serializer.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutMapListObjectsWithPHPSerializer");
--EXPECT--
OK
