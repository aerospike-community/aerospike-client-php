--TEST--
Info - Positive for mesh

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForMesh");
--EXPECT--
OK
