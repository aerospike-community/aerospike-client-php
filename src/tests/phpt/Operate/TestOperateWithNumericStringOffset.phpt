--TEST--
Operate - bin value by Numeric String

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "TestOperateWithNumericStringOffset");
--EXPECT--
OK
