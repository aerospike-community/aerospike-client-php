<?php
include(dirname(__FILE__).'/tests/aerospike.inc');

function servers_present($response1, $response2) {
    if (strpos($response1, 'cluster_size=2') === false) return false;
    if (strpos($response2, 'cluster_size=2') === false) return false;
    $start1 = strpos($response1, 'cluster_key=') + 12;
    $end1 = strpos($response1, ';', $start1);
    $len1 = $end1 - $start1;
    $cluster_key1 = substr($response1, $start1, $len1);
    $start2 = strpos($response2, 'cluster_key=') + 12;
    $end2 = strpos($response2, ';', $start2);
    $len2 = $end2 - $start2;
    $cluster_key2 = substr($response2, $start2, $len2);
    return ($cluster_key1 == $cluster_key2);
}

$config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
$db = new Aerospike($config, false);
if (defined('AEROSPIKE_CONFIG_NAME2') && defined('AEROSPIKE_CONFIG_PORT2')) {
    $config['hosts'][] = array("addr"=>AEROSPIKE_CONFIG_NAME2, "port"=>AEROSPIKE_CONFIG_PORT2);
}

echo "Waiting on the Aerospike cluster..";
$keep_waiting = true;
while ($keep_waiting) {
    $host1_status = $db->info("statistics", $host1_response, $config['hosts'][0]);
    $host2_status = $db->info("statistics", $host2_response, $config['hosts'][1]);

    if ($host1_status === Aerospike::OK && $host2_status === Aerospike::OK) {
        if (servers_present($host1_response, $host2_response)) {
            $keep_waiting = false;
            break;
        }
    }
    echo ".";
    sleep(1);
}
echo "OK\n";
$db->close();
?>
