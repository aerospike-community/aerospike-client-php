
# Aerospike::createRole

Aerospike::createRole - creates a new user defined role in a security-enabled Aerospike database

## Description

```
public int Aerospike::createRole ( string $role, array $privileges [, array $options ] )
```

**Aerospike::createRole()** will create a user defined role with specified privileges.
Clear-text password will be hashed using bcrypt before sending to server.

## Parameters

**role** string rolename.

**privileges** an array of Aerospike::PRIV\_* privileges to be assigned to the role.

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

$res = $client->createRole("example_role", array(array("code" : Aerospike::PRIV_READ, "ns":
                "test", "set": "demo")));
if ($res == Aerospike::OK) {
    echo "Role successfully created";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
Role successfully created
```
