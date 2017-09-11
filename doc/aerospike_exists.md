
# Aerospike::exists

Aerospike::exists - check if a record exists in the Aerospike database

## Description

```
public int Aerospike::exists ( array $key, array &$metadata [, array $options ] )
```

**Aerospike::exists()** will check if a record with a given *key* exists in the database.
If such a key exists its metadata will be returned in the *metadata* variable,
otherwise it will be NULL.

## Parameters

**key** the key under which the record can be found. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**metadata** filled by an array of metadata.

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**
- **[Aerospike::OPT_POLICY_KEY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9c8a79b2ab9d3812876c3ec5d1d50ec)**
- **[Aerospike::OPT_POLICY_CONSISTENCY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga34dbe8d01c941be845145af643f9b5ab)**
- **[Aerospike::OPT_POLICY_REPLICA](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gabce1fb468ee9cbfe54b7ab834cec79ab)**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = $client->initKey("test", "users", 1234);
$status = $client->exists($key, $metadata);
if ($status == Aerospike::OK) {
    var_dump($metadata);
} elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
    echo "A user with key ". $key['key']. " does not exist in the database\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(2) {
  ["generation"]=>
  int(4)
  ["ttl"]=>
  int(1337)
}
```
**or**
```
A user with key 1234 does not exist in the database.
```

