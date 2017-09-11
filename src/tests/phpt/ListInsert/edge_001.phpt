--TEST--
listInsert() with key is of type an integer

--DESCRIPTION--
This testcase will test for key is of type an integer.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ListInsert", "edge_001");
--EXPECT--
ERR_PARAM
