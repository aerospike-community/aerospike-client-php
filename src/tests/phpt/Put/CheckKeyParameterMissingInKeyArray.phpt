--TEST--
Put - Key Parameter missing in key array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testCheckKeyParameterMissingInKeyArray");
--EXPECT--
ERR_PARAM
