--TEST--
listMerge() with items parameter is of type string

--DESCRIPTION--
This testcase will test for items parameter is of type string.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListMerge", "edge_003");
--EXPECT--
ERR_PARAM
