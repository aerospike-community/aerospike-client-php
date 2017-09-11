--TEST--
listRemoveRange() with bin is of type an array

--DESCRIPTION--
This testcase will test for bin is of type an array.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListRemoveRange", "edge_002");
--EXPECT--
ERR_PARAM
