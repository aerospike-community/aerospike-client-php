--TEST--
GetMany - correct arguments, without options and filter bins, with one non-existent key

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany","testGetManyWithNonExistentKeyPositive");
--EXPECT--
OK

