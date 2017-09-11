
# Aerospike::queryRole

Aerospike::queryRole - Query a given user defined role in a security-enabled Aerospike database

## Description

```
public int queryRole ( string $role, array &$privileges [, array $options ] )
```

**Aerospike::queryRole()** will query  a user defined role.

## Parameters

**role** string rolename.

**privileges** an array of privileges possessed by the role.

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
   echo "Aerospike failed to connect[{$client->errorno()} {$client->error()}";
   exit(1);
}

$res = $client->grantPrivileges("example_role", [["code": Aerospike::PRIV_READ]]);
if ($res == Aerospike::OK) {
    echo "read privileges successfully granted to role";
    $res = $client->queryRole("example_role", $privileges);
    if ($res == Aerospike::OK) {
        var_dump($privileges);
    } else {
        echo "[{$client->errorno()}] ".$client->error();
    }
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
read privileges successfully granted to role
array(2) {
  [0]=>
  array(3) {
    ["ns"]=>
    string(0) ""
    ["set"]=>
    string(0) ""
    ["code"]=>
    int(10)
  }
  [1]=>
  array(3) {
    ["ns"]=>
    string(4) "test"
    ["set"]=>
    string(4) "demo"
    ["code"]=>
    int(11)
  }
}
```
