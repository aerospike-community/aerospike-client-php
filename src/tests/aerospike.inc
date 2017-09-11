<?php
// You can overwrite these values without modifying the current file by
// creating a local config file (aerospike.local.inc) which defines
// AEROSPIKE_CONFIG_* constants as seen in the else clause
if(file_exists(dirname(__FILE__)."/aerospike.local.inc")) {
    include(dirname(__FILE__)."/aerospike.local.inc");
} else {
    // modify the address and port information according to your config
    define("AEROSPIKE_CONFIG_NAME", "127.0.0.1");
    define("AEROSPIKE_CONFIG_PORT", 3000);

    // If you are creating a local config file (aerospike.local.inc) you may
    // skip multi-host configuration tests by commenting out the following:
    define("AEROSPIKE_CONFIG_NAME2", "127.0.0.1");
    define("AEROSPIKE_CONFIG_PORT2", 3010);

    // Database is Enterprise Edition, so run security tests
    define("AEROSPIKE_ENTERPRISE_EDITION", false);
}
?>
