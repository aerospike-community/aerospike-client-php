--TEST--
ScanApply - Unknown Namespace and Set

--FILE--
<?php
include dirname(__FILE__)."/../../astestframework/astest-phpt-loader.inc";
aerospike_phpt_runtest("ScanApply", "testScanApplyUnknownNamespaceAndSetNegative");
--EXPECTREGEX--
(ERR_NAMESPACE_NOT_FOUND|ERR_SERVER)

