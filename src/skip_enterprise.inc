<?php
include(dirname(__FILE__)).'/tests/aerospike.inc';
if (!defined('AEROSPIKE_ENTERPRISE_EDITION') ||  constant('AEROSPIKE_ENTERPRISE_EDITION') !== true) {
    die("SKIP config states server is not Enterprise Edition.");
}
