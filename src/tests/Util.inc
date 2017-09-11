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
?>
