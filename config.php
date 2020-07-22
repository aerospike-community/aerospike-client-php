<?php
$options = [
   Aerospike::OPT_CONNECT_TIMEOUT => 300,
   Aerospike::OPT_READ_TIMEOUT => 300
];
$config = [
	"hosts" => [
 	   ["addr" => "199.115.119.53", "port" => 3000 ],
           ['addr' => '23.82.8.40', 'port' => 3000],
           ['addr' => '199.115.119.12', 'port' => 3000],
           ['addr' => '199.115.119.52', 'port' => 3000],
           ['addr' => '199.115.119.15', 'port' => 3000],
           ['addr' => '199.115.119.43', 'port' => 3000]],
// 	"use_services_alternate" => True
	"options" => [
          Aerospike::OPT_CONNECT_TIMEOUT => 300,
          Aerospike::OPT_READ_TIMEOUT => 300,
	  Aerospike::OPT_USE_SERVICES_ALTERNATES => true
        ],
];
echo ("set config\n");
$client = new Aerospike($config, false, $options);
if (!$client->isConnected()) {
  echo "Failed to connect to the Aerospike server [{$db->errorno()}]: {$db->error()}\n";
  exit(1);
}
else {
 echo ("i'm in\n");
}
echo ("client configured\n");
$key = $client->initKey("clearsky", "impid", "15953406911768375583225911221560613");
echo ("impid init done\n");
//var_dump($key);
$status = $client->get($key, $record, ['json']);
echo ("got record\n");
//var_dump($record);
exit(0)
?>
