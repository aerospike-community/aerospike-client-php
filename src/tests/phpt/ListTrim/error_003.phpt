--TEST--
listTrim() with non-existent index

--DESCRIPTION--
This testcase will test for listTrim() with non-existent index.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListTrim", "error_003");
--EXPECTREGEX--
(ERR_REQUEST_INVALID|OK)
