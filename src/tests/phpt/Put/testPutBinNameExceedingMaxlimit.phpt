--TEST--
Basic PUT with bin name exceeding 14 characters limit.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPutBinNameExceedingMaxlimit");
--EXPECT--
ERR_BIN_NAME
