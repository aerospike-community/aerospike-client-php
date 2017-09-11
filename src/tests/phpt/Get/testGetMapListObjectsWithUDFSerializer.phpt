--TEST--
Get Map containing List of objects with UDF Serializer.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Get", "testGetMapListObjectsWithUDFSerializer");
--EXPECT--
OK
