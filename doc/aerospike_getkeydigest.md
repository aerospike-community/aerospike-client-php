
# Aerospike::getKeyDigest

Aerospike::getKeyDigest - helper method for building the key array

## Description

```
public string Aerospike::getKeyDigest (string $ns, string $set, string|int $pk)
```

**Aerospike::getKeyDigest()** will return the RIPEMD-160 digest corresponding to
the hash of the key tuple. The digest is used to uniquely identify the record in
the cluster.

## Parameters

**ns** the namespace

**set** the name of the set within the namespace

**pk** the primary key that identifies the record in the application

## Return Value

The RIPEMD-160 digest, stored as a binary string. NULL on error.

## Examples

### Initializing a key
```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$digest = $client->getKeyDigest("test", "users", 1);
$key = $client->initKey("test", "users", $digest, true);
var_dump($digest, $key);

?>
```

We expect to see:

```
string(20) "9!?@%??;???Wp?'??Ag"
array(3) {
  ["ns"]=>
  string(4) "test"
  ["set"]=>
  string(5) "users"
  ["digest"]=>
  string(20) "9!?@%??;???Wp?'??Ag"
}
```

