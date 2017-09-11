--TEST--
put() Invalid GeoJson - Lower cased 'point'

--DESCRIPTION--
write Invalid GeoJson - Lower cased 'point' returns ERR_GEO_INVALID_GEOJSON error

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "testPutWithInvalidJSONString");
?>

--EXPECT--
ERR_GEO_INVALID_GEOJSON
