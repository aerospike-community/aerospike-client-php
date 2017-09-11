
# Aerospike::removeBin

Aerospike::removeBin - removes a bin from a record

## Description

```
public int Aerospike::removeBin ( array $key, array $bins [, array $options ] )
```

**Aerospike::removeBin()** will remove the specified *bins* from the record* with
 a given *key*. Like other bin operations, removeBin() only works on existing
records (i.e. ones that were previously created with a put()).

## Parameters

**key** the key for the record. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**bins** the name of the bins to be removed from the record.

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**
- **Aerospike::OPT_TTL**
- **[Aerospike::OPT_POLICY_RETRY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9730980a8b0eda8ab936a48009a6718)**
- **[Aerospike::OPT_POLICY_KEY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9c8a79b2ab9d3812876c3ec5d1d50ec)**
- **[Aerospike::OPT_POLICY_GEN](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga38c1a40903e463e5d0af0141e8c64061)**
- **[Aerospike::OPT_POLICY_COMMIT_LEVEL](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga17faf52aeb845998e14ba0f3745e8f23)**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = ["ns" => "test", "set" => "users", "key" => 1234];
$options = array(Aerospike::OPT_TTL => 3600);
$status = $client->removeBin($key, ["age"], $options);
if ($status == Aerospike::OK) {
    echo "Removed bin 'age' from the record.\n";
} elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
    echo "The database has no record with the given key.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
Removed bin 'age' from the record.
```

or

```
The database has no record with the given key.
```

