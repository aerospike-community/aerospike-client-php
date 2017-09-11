
# Aerospike::getNodes

Aerospike::getNodes - get the addresses of the cluster nodes

## Description

```
public array Aerospike::getNodes ( void )
```

**Aerospike::getNodes()** will return an array of cluster node addresses.

## Parameters

This method has no parameters.

## Return Values

Returns an array with the following structure:
```
Array:
  Array:
    'addr' => the IP address of the node
    'port' => the port of the node
```

## Examples

```php
<?php


$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$nodes = $client->getNodes();
var_dump($nodes);
?>
```

We expect to see:

```
array(2) {
  [0]=>
  array(2) {
    ["addr"]=>
    string(15) "192.168.120.145"
    ["port"]=>
    string(4) "3000"
  }
  [1]=>
  array(2) {
    ["addr"]=>
    string(15) "192.168.120.144"
    ["port"]=>
    string(4) "3000"
  }
}
```

