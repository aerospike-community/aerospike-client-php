--TEST--
GetMany - correct arguments, with empty filter bins and without options

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyPositiveWithEmptyFilterBins");
--EXPECT--
OK

