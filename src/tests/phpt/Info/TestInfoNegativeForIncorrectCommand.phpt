--TEST--
Info - Check for incorrect command name

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoNegativeForIncorrectCommand");
--EXPECT--
OK
