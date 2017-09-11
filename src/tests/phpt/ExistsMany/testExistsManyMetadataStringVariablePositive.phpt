--TEST--
 Basic existsMany, Metadata is of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany",
"testExistsManyMetadataStringVariablePositive");
--EXPECT--
OK
