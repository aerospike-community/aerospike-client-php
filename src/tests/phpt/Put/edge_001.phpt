--TEST--
put() with only bin name is given

--DESCRIPTION--
This testcase will test for put() with only bin name is given, Value is not
given.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "edge_001");
?>
--EXPECT--
ERR_PARAM
