--TEST--
 Basic existsMany, Metadata is empty string variable.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ExistsMany", "testExistsManyMetadataEmptyStringPositive");
--EXPECT--
OK
