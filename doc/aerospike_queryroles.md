
# Aerospike::queryRoles

Aerospike::queryRoles - Query all roles in a security-enabled Aerospike database

## Description

```
public int queryRoles ( array &$roles [, array $options ] )
```

**Aerospike::queryRoles()** will query  all user defined roles.

## Parameters

**roles** an array of privileges possessed by each role.

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

$res = $client->grantPrivileges("example_role", array(array("code": Aerospike::PRIV_READ)));
if ($res == Aerospike::OK) {
    echo "read privileges successfully granted to role";
    $res = $client->queryRoles($privileges);
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
array(6) {
  ["example_role"]=>
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
  ["read"]=>
  array(1) {
    [0]=>
    array(3) {
      ["ns"]=>
      string(0) ""
      ["set"]=>
      string(0) ""
      ["code"]=>
      int(10)
    }
  }
  ["read-write"]=>
  array(1) {
    [0]=>
    array(3) {
      ["ns"]=>
      string(0) ""
      ["set"]=>
      string(0) ""
      ["code"]=>
      int(11)
    }
  }
  ["read-write-udf"]=>
  array(1) {
    [0]=>
    array(3) {
      ["ns"]=>
      string(0) ""
      ["set"]=>
      string(0) ""
      ["code"]=>
      int(12)
    }
  }
  ["sys-admin"]=>
  array(1) {
    [0]=>
    array(3) {
      ["ns"]=>
      string(0) ""
      ["set"]=>
      string(0) ""
      ["code"]=>
      int(1)
    }
  }
  ["user-admin"]=>
  array(1) {
    [0]=>
    array(3) {
      ["ns"]=>
      string(0) ""
      ["set"]=>
      string(0) ""
      ["code"]=>
      int(0)
    }
  }
}
```
