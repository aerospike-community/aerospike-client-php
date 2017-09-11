--TEST--
GetMany - with empty string for key array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("GetMany", "testGetManyKeyEmptyStringNegative");
--EXPECT--
ERR_PARAM

