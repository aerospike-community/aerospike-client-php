--TEST--
query() on geoJSON data with predicate predicateGeoContainsPoint() 

--DESCRIPTION--
query() on geoJSON data with predicate predicateGeoContainsPoint() 

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "testQueryGeoContainsPoint");
?>

--EXPECT--
OK
