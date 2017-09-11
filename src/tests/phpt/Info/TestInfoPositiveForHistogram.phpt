--TEST--
Info - Positive for histogram

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoPositiveForHistogram");
--EXPECT--
OK
