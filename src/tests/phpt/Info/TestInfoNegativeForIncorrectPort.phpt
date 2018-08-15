--TEST--
Info - Check for incorrect port no

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Info", "testInfoNegativeForIncorrectPort");
--EXPECTREGEX--
(ERR_TIMEOUT|ERR_CLIENT|ERR_CONNECTION)
