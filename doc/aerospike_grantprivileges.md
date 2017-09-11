
# Aerospike::grantPrivileges

Aerospike::grantPrivileges - Add privileges to role's list of privileges in a security-enabled Aerospike database

## Description

```
public int grantPrivileges ( string $role, array $privileges [, array $options ] )
```

**Aerospike::grantPrivileges()** will add privileges to a role's list of privileges.

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

$res = $client->grantPrivileges("example_role", array(array("code":
                Aerospike::PRIV_READ_WRITE_UDF)));
if ($res == Aerospike::OK) {
    echo "read-write_udf privileges successfully granted to role";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
read-write_udf privileges successfully granted to role
```
