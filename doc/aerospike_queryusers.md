
# Aerospike::queryUsers

Aerospike::queryUsers - Retrieve all users and their roles in a security-enabled Aerospike database

## Description

```
public int queryUsers ( array &$roles [, array $options ] )
```

**Aerospike::queryUsers()** will retrieve all users and their roles.

## Parameters

**roles** an array of roles possessed by the user populated by this method.

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

$res = $client->createUser("john", "john@123", array("read-write"));
$res = $client->createUser("steve", "steve@123", array("user-admin", "sys-admin"));
if ($res == Aerospike::OK) {
    $res = $client->queryUsers($roles);
    if ($res == Aerospike::OK) {
        var_dump($roles);
    } else {
        echo "[{$client->errorno()}] ".$client->error();
    }
} elseif ($res == Aerospike::ERR_ROLE_VIOLATION) {
    echo "User does not possess the required role to create user";
} elseif ($res == Aerospike::ERR_INVALID_ROLE) {
    echo "Invalid Role being attempted to be assigned to user";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(2) {
  ["john"]=>
    array(1) {
      [0]=>
        string(10) "read-write"
    }
  ["steve"]=>
    array(2) {
      [0]=>
        string(10) "user-admin"
      [1]=>
        string(9) "sys-admin"
    }
}
```

