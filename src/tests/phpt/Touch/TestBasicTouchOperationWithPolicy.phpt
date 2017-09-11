--TEST--
Touch - Basic Touch opeartion with policy

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Touch", "testBasicTouchOperationWithPolicy");
--EXPECT--
OK
