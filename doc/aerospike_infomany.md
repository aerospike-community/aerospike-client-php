
# Aerospike::infoMany

Aerospike::info - send an info request to multiple cluster nodes.

## Description

```
public array Aerospike::infoMany ( string $request [, array $config [, array options ]] )

```

**Aerospike::infoMany()** interfaces with the Aerospike cluster's command and
control functions.  A [formatted](http://www.aerospike.com/docs/tools/asinfo/)
*request* string is sent to all or some specified cluster nodes, and a
formatted response returned as an array.

A specific *config* of hosts can be optionally set, otherwise the request
command is sent to the all the nodes in the cluster.

## Parameters

**request** a formatted string representing a command and control operation.

**config** an array holding the cluster node connection information
cluster and manage its connections to them.

- *hosts* an array of host data
  - *addr* hostname or IP of the node
  - *port*

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**

## Return Values

Returns an array of response strings keyed by cluster node ID. If an error
occurs waiting on a response from a node it will be returned as a null.
```
Array:
  NODE-ID => response string
```

## See Also

- [Aerospike Information Tool (asinfo)](http://www.aerospike.com/docs/tools/asinfo/)
- [Aerospike::info()](aerospike_info.md)

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$response = $client->infoMany('build');
var_dump($response);

?>
```

We expect to see:

```
array(3) {
  ["BB936F106CA0568"]=>
  string(6) "build  3.3.19"
  ["AE712F245BB9876"]=>
  string(6) "build  3.3.19"
  ["DCBA9AA34EE12FA"]=>
  string(6) "build  3.3.19"
}
```

