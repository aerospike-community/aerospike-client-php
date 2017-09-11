
# Aerospike::dropUser

Aerospike::dropUser - Drops an existing user in a security-enabled Aerospike database

## Description

```
public int Aerospike::dropUser ( string $user [, array $options ] )
```

**Aerospike::dropUser()** will drop an existing user from the cluster.

## Parameters

**user** string username.

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Example

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]],
           "shm"=>[],
           "user"=>"admin", "pass"=>"admin"];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$res = $client->dropUser("john");
if ($res == Aerospike::OK) {
    echo "User john successfully removed";
} elseif ($res == Aerospike::ERR_INVALID_USER) {
    echo "User john does not exist";
} elseif ($res == Aerospike::ERR_ROLE_VIOLATION) {
    echo "User does not possess the required role to drop a user";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
User john successfully removed
```

