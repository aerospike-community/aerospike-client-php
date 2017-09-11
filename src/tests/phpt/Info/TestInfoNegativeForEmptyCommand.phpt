--TEST--
Info - Check for empty command name

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoNegativeForEmptyCommand");
--EXPECT--
OK
