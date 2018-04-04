--TEST--
listTrim() with non-existent bin

--DESCRIPTION--
This testcase will test for listTrim() with non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListTrim", "error_002");
--EXPECTREGEX--
(ERR_BIN_INCOMPATIBLE_TYPE|OK)
