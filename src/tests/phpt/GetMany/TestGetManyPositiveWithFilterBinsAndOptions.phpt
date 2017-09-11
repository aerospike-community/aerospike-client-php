--TEST--
GetMany - correct arguments, with filter bins and options

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyPositiveWithFilterBinsAndOptions");
--EXPECT--
OK

