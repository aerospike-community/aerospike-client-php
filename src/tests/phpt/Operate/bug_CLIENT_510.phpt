--TEST--
operate() with OPT_TTL option

--DESCRIPTION--
This testcase will perform OPERATOR_APPEND and OPERATOR_READ operations along
with OPT_TTL is set to 10. After 10 milliseconds record will be get removed from
the database.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "bug_CLIENT_510");
--EXPECT--
OK
