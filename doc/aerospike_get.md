
# Aerospike::get

Aerospike::get - gets a record from the Aerospike database

## Description

```
public int Aerospike::get ( array $key, array &$record [, array $select [, array $options]] )
```

**Aerospike::get()** will read a *record* with a given *key*, where the *record*
is filled with an array of bins and values.  The bins returned in
*record* can be filtered by passing a *select* array of bin names.
Non-existent bins will not appear in the record.

## Parameters

**key** the key under which the record can be found. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**record** an array of key, metadata, and bins:
```
Array:
  key => Array
    ns => namespace
    set => set name
    key => the primary index key
    digest => a RIPEMD-160 hash of the key, and always present
  metadata => Array
    ttl => time in seconds until the record expires
    generation => reflects the number of times the record has been altered
  bins => Array of bin-name => value pairs
```

**select** an array of bin names which are the subset to be returned.

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

### Example #1 Aerospike::get() default behavior example

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = $client->initKey("test", "users", 1234);
$status = $client->get($key, $record);
if ($status == Aerospike::OK) {
    var_dump($record);
} elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
    echo "A user with key ". $key['key']. " does not exist in the database\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(3) {
  ["key"]=>
  array(4) {
    ["digest"]=>
    string(40) "436a3b9fcafb96d12844ab1377c0ff0d7a0b70cc"
    ["namespace"]=>
    NULL
    ["set"]=>
    NULL
    ["key"]=>
    NULL
  }
  ["metadata"]=>
  array(2) {
    ["generation"]=>
    int(3)
    ["ttl"]=>
    int(12345)
  }
  ["bins"]=>
  array(3) {
    ["email"]=>
    string(9) "hey@example.com"
    ["name"]=>
    string(9) "You There"
    ["age"]=>
    int(33)
  }
}
```

### Example #2 get the record with filtered bins

```php
<?php

// assuming this follows Example #1

// Getting a filtered record
$filter = ["email", "manager"];
unset($record);
$status = $client->get($key, $record, $filter);
if ($status == Aerospike::OK) {
    var_dump($record);
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(3) {
  ["key"]=>
  array(4) {
    ["digest"]=>
    string(40) "436a3b9fcafb96d12844ab1377c0ff0d7a0b70cc"
    ["namespace"]=>
    NULL
    ["set"]=>
    NULL
    ["key"]=>
    NULL
  }
  ["metadata"]=>
  array(2) {
    ["generation"]=>
    int(3)
    ["ttl"]=>
    int(12344)
  }
  ["bins"]=>
  array(1) {
    ["email"]=>
    string(15) "hey@example.com"
  }
}
```

## See Also

- [Aerospike Data Model](http://www.aerospike.com/docs/architecture/data-model.html)
- [Key-Value Store](http://www.aerospike.com/docs/guide/kvs.html)
- [Glossary](http://www.aerospike.com/docs/guide/glossary.html)
