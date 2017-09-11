
# Info Methods

### [Aerospike::getNodes](aerospike_getnodes.md)
```
public array Aerospike::getNodes ( void )
```

### [Aerospike::info](aerospike_info.md)
```
public int Aerospike::info ( string $request, string &$response [, array $host ] )
```

### [Aerospike::infoMany](aerospike_infomany.md)
```
public array Aerospike::infoMany ( string $request [, array $config ] )
```

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$opts = [Aerospike::OPT_CONNECT_TIMEOUT => 1250];
$client = new Aerospike($config, true, $opts);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$status = $client->info('bins/test', $response);
if ($status == Aerospike::OK) {
    var_dump($response);
}

// Get the nodes in the cluster
$nodes = $client->getNodes();
var_dump($nodes);

?>
```
