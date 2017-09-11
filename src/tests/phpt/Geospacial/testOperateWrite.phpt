--TEST--
 Check if operate write works properly.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "testOperateWrite");
--EXPECT--
OK
