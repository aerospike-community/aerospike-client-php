
# Aerospike::revokeRoles

Aerospike::revokeRoles - Remove roles from user's list of roles in a security-enabled Aerospike database

## Description

```
public int revokeRoles ( string $user, array $roles [, array $options ] )
```

**Aerospike::revokeRoles()** will remove roles from user's list of existing roles.

## Parameters

**user** string username.

**roles** an array of roles to be revoked from existing roles of the user.

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

$res = $client->revokeRoles("john", array("user-admin", "sys-admin"));
if ($res == Aerospike::OK) {
    echo "User john's user-admin and sys-admin roles have been successfully revoked";
} elseif ($res == Aerospike::ERR_ROLE_VIOLATION) {
    echo "User does not possess the required role to revoke roles";
} elseif ($res == Aerospike::ERR_INVALID_ROLE) {
    echo "Invalid Role being attempted to be revoked";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
User john's user-admin and sys-admin roles have been successfully revoked
```

