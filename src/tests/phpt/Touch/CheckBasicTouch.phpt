--TEST--
Touch - Basic Touch opeartion

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Touch", "testBasicTouchOperation");
--EXPECT--
OK
