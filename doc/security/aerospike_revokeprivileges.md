
# Aerospike::revokePrivileges

Aerospike::revokePrivileges - Revoke privileges from a role's list of privileges in a security-enabled Aerospike database

## Description

```
public int revokePrivileges ( string $role, array $privileges [, array $options ] )
```

**Aerospike::revokePrivileges()** will revoke privileges from a role's list of privileges.

## Parameters

**role** string rolename.

**privileges** an array of old Aerospike::PRIV\_* privileges to be revoked from the role.

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

$res = $client->revokePrivileges("example_role", array(array("code":
                Aerospike::PRIV_READ_WRITE_UDF)));
if ($res == Aerospike::OK) {
    echo "read-write_udf privileges successfully revoked from role";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
read-write_udf privileges successfully revoked from role
```
