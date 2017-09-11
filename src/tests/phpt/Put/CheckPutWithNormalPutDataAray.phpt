--TEST--
Put - data as normal array ex. array("hi","hello");

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("Put", "testPUTWithNormalpPutDataArrayParameter");
--EXPECT--
ERR_PARAM
