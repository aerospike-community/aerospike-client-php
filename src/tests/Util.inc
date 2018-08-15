<?php
require_once dirname(__FILE__).'/aerospike.inc';
function make_handle($persistent = true) {
$config = array("hosts"=>array(array("name"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
    $handle = new Aerospike($config);
    return $handle;
}

function make_handle_oo($persistent = true) {
$config = array("hosts"=>array(array("name"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
    $oo = new Aerospike($config);

    return $oo;
}

function check_for_socket($addr, $port, $timeout_s = 10) {
    return @fsockopen($addr, 3000, $errno, $errstr, $timeout_s);
}

function array_diff_assoc_recursive($array1, $array2) {
    $difference=array();
    foreach($array1 as $key => $value) {
        if( is_array($value) ) {
            if( !isset($array2[$key]) || !is_array($array2[$key]) ) {
                $difference[$key] = $value;
            } else {
                $new_diff = array_diff_assoc_recursive($value, $array2[$key]);
                if( !empty($new_diff) )
                    $difference[$key] = $new_diff;
            }
        } else if( !array_key_exists($key,$array2) || $array2[$key] != $value )     {
            $difference[$key] = $value;
        }
    }
    return $difference;
}

function get_as_config() {

    $host = ['addr' => AEROSPIKE_CONFIG_NAME, 'port' => AEROSPIKE_CONFIG_PORT];
    if (getenv("CLIENT_TEST_TLSNAME") != "") {
        $host["tlst_name"] = getenv("CLIENT_TEST_TLSNAME");
    }

    //var_dump($host);
    $config = ["hosts"=>[$host]];
    $tls_config = [];
    $use_tls = false;
    if (defined('AEROSPIKE_USE_TLS') && constant('AEROSPIKE_USE_TLS')) {
        if (getenv("TLS_ENABLE") != "") {
            $tls_config[Aerospike::OPT_TLS_ENABLE] = (int)getenv("TLS_ENABLE");
            $use_tls = (int)getenv("TLS_ENABLE");
        }
        if (getenv("TLS_CIPHERSUITE") != "") {
            $tls_config[Aerospike::OPT_TLS_CIPHER_SUITE] = getenv("TLS_CIPHERSUITE");
        }
        if (getenv("TLS_CAFILE") != "") {
            $tls_config[Aerospike::OPT_TLS_CAFILE] = getenv("TLS_CAFILE");
        }
        if (getenv("TLS_KEYFILE") != "") {
            $tls_config[Aerospike::OPT_TLS_KEYFILE] = getenv("TLS_KEYFILE");
        }
        if (getenv("TLS_CERTFILE") != "") {
            $tls_config[Aerospike::OPT_TLS_CERTFILE] = getenv("TLS_CERTFILE");
        }
        if (getenv("TLS_CERTBLACKLIST") != "") {
            $tls_config[Aerospike::OPT_TLS_CERT_BLACKLIST] = getenv("TLS_CERTBLACKLIST");
        }
    }
    if ($use_tls) {
        $config[Aerospike::OPT_TLS_CONFIG] = $tls_config;
    }

    return $config;
}
?>
