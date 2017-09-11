--TEST--
 Check if proper predicate is formed.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Geospatial", "testPredicateGeoWithinRadius");
--EXPECT--
OK
