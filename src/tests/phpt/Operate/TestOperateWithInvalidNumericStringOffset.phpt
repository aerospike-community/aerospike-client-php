--TEST--
Operate - bin value by Invalid Numeric String

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Operate", "TestOperateWithInvalidNumericStringOffset");
--EXPECT--
ERR_PARAM
