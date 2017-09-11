
# Security Methods

### [Aerospike::createRole](aerospike_createrole.md)
```
public int Aerospike::createRole ( string $role, array $privileges [, array $options ] )
```

### [Aerospike::grantPrivileges](aerospike_grantprivileges.md)
```
public int Aerospike::grantPrivileges ( string $role, array $privileges [, array $options ] )
```

### [Aerospike::revokePrivileges](aerospike_revokeprivileges.md)
```
public int Aerospike::revokePrivileges ( string $role, array $privileges [, array $options ] )
```

### [Aerospike::queryRole](aerospike_queryrole.md)
```
public int Aerospike::queryRole ( string $role, array &$privileges [, array $options ] )
```

### [Aerospike::queryRoles](aerospike_queryroles.md)
```
public int Aerospike::queryRoles ( array &$roles [, array $options ] )
```

### [Aerospike::dropRole](aerospike_droprole.md)
```
public int Aerospike::dropRole ( string $role [, array $options ] )
```

### [Aerospike::createUser](aerospike_createuser.md)
```
public int Aerospike::createUser ( string $user, string $password, array $roles [, array $options ] )
```

### [Aerospike::changePassword](aerospike_changepassword.md)
```
public int Aerospike::changePassword ( string $user, string $password [, array $options ] )
```

### [Aerospike::grantRoles](aerospike_grantroles.md)
```
public int Aerospike::grantRoles ( string $user, array $roles [, array $options ] )
```

### [Aerospike::revokeRoles](aerospike_revokeroles.md)
```
public int Aerospike::revokeRoles ( string $user, array $roles [, array $options ] )
```

### [Aerospike::queryUser](aerospike_queryuser.md)
```
public int Aerospike::queryUser ( string $user, array &$roles [, array $options ] )
```

### [Aerospike::queryUsers](aerospike_queryusers.md)
```
public int Aerospike::queryUsers ( array &$roles [, array $options ] )
```

### [Aerospike::dropUser](aerospike_dropuser.md)
```
public int Aerospike::dropUser ( string $user [, array $options ] )
```

## Example

```php
<?php

$config = [
  "hosts" => [
    ["addr" => "localhost", "port" => 3000]],
  "shm"=> [],
  "user" => "admin",
  "pass" => "admin"];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$res = $client->createUser("john", "john@123", ["read-write"]);
$res = $client->createUser("steve", "steve@123", ["user-admin", "sys-admin"]);
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
