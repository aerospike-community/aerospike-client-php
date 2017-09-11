--TEST--
removeBin() with OPT_TTL option

--DESCRIPTION--
This testcase will remove a bin from record and OPT_TTL is set to 10.
After 10 milliseconds the whole record will be get removed from the database.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Remove", "bug_CLIENT_510");
--EXPECT--
OK
