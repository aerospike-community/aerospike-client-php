--TEST--
 Check if proper geojson object is put in the database.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "geojsonPutGet");
--EXPECT--
OK
