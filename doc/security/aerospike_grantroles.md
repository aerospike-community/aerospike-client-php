
# Aerospike::grantRoles

Aerospike::grantRoles - Add roles to user's list of roles in a security-enabled Aerospike database

## Description

```
public int grantRoles ( string $user, array $roles [, array $options ] )
```

**Aerospike::grantRoles()** will add roles to a user's list of roles.

## Parameters

**user** string username.

**roles** an array of new roles to be assigned to the user.

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

$res = $client->grantRoles("john", array("read-write", "sys-admin"));
if ($res == Aerospike::OK) {
    echo "read-write and sys-admin roles successfully granted to user john";
} elseif ($res == Aerospike::ERR_ROLE_VIOLATION) {
    echo "User does not possess the required role to grant new roles";
} elseif ($res == Aerospike::ERR_INVALID_ROLE) {
    echo "Invalid Role being attempted to be assigned to user john";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
read-write and sys-admin roles successfully granted to user john
```

