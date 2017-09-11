--TEST--
query() on geoJSON data with predicate predicateGeoContainsGeoJSONPoint() 

--DESRIPTION--
query() on geoJSON data with predicate predicateGeoContainsGeoJSONPoint() 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "testQueryGeoContainsGeoJSONPoint");
?>

--EXPECT--
OK
