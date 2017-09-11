--TEST--
operateOrdered() with val parameter is missing for OPERATOR_WRITE.

--DESCRIPTION--
This testcase will test for operateOrdered() with val parameter is missing for OPERATOR_WRITE.

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("OperateOrdered", "edge_004");
--EXPECT--
ERR_PARAM
