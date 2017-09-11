--TEST--
PUT with initkey digest and option is POLICY_KEY_DIGEST.  

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put",
"testPutInitKeyWithDigestAndOptionKeyDigestPositive");
--EXPECT--
OK
