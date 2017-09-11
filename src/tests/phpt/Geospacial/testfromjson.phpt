--TEST--
 Check if passed a json string as parameter, proper geojson object is returned.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "fromJson");
--EXPECT--
OK
