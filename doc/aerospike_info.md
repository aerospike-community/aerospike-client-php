
# Aerospike::info

Aerospike::info - send an info request to a single cluster node

## Description

```
public int Aerospike::info ( string $request, string &$response [, array $host [, array options ] ] )

```

**Aerospike::info()** interfaces with the Aerospike cluster's command and
control functions.  A [formatted](http://www.aerospike.com/docs/tools/asinfo/)
*request* string is sent to a cluster node, and a formatted *response* returned.

A specific *host* can be optionally set, otherwise the request command is sent
to the host definded for the **Aerospike__construct()** instantiation.

## Parameters

**request** a formatted string representing a command and control operation.

**response** a formatted response from the server.

**host** an array holding the cluster node connection information
cluster and manage its connections to them.

- *addr* hostname or IP of the node
- *port*

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## See Also

- [Aerospike Information Tool (asinfo)](http://www.aerospike.com/docs/tools/asinfo/)
- [Aerospike::getNodes()](aerospike_getnodes.md)

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$status = $client->info('bins/test', $response);
if ($status == Aerospike::OK) {
    var_dump($response);
}

?>
```

We expect to see:

```
string(53) "bins/test	num-bin-names=2,bin-names-quota=32768,demo,characters"
```

