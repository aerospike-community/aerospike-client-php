--TEST--
 PUT with initkey digest and option is POLICY_KEY_SEND.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutInitKeyWithDigestAndOptionKeySendPositive");
--EXPECT--
OK
