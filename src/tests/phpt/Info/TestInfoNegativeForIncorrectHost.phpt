--TEST--
Info - Check for incorrect host

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoNegativeForIncorrectHost");
--EXPECT--
ERR_INVALID_HOST
