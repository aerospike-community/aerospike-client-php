--TEST--
listPop() with non-existent bin

--DESCRIPTION--
This testcase will test for listPop() with non-existent bin.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListPop", "error_002");
--EXPECTREGEX--
(ERR_BIN_INCOMPATIBLE_TYPE|OK)
