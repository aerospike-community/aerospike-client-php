--TEST--
Try putting NULL record without any bins (example: [NULL])

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutNullRecordNegative");
--EXPECT--
ERR_PARAM
