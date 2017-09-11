
# Aerospike::touch

Aerospike::touch - touch a record in the Aerospike DB

## Description

```
public int Aerospike::touch ( array $key, int $ttl = 0 [, array $options ] )
```

**Aerospike::touch()** will touch the given record, resetting its time-to-live
and incrementing its generation.

## Parameters

**key** the key for the record. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**ttl** the [time-to-live](http://www.aerospike.com/docs/client/c/usage/kvs/write.html#change-record-time-to-live-ttl) in seconds for the record.

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**
- **[Aerospike::OPT_POLICY_RETRY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9730980a8b0eda8ab936a48009a6718)**
- **[Aerospike::OPT_POLICY_KEY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9c8a79b2ab9d3812876c3ec5d1d50ec)**
- **[Aerospike::OPT_POLICY_GEN](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga38c1a40903e463e5d0af0141e8c64061)**
- **[Aerospike::OPT_POLICY_REPLICA](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gabce1fb468ee9cbfe54b7ab834cec79ab)**
- **[Aerospike::OPT_POLICY_CONSISTENCY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga34dbe8d01c941be845145af643f9b5ab)**
- **[Aerospike::OPT_POLICY_COMMIT_LEVEL](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga17faf52aeb845998e14ba0f3745e8f23)**

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
$status = $client->touch($key, 120);
if ($status == Aerospike::OK) {
    echo "Set the record's ttl to 120.\n";
} elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
    echo "A user with key ". $key['key']. " does not exist in the database\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
Set the record's ttl to 120.
```
**or**
```
A user with key 1234 does not exist in the database.
```

## See Also

- [FAQ](https://www.aerospike.com/docs/guide/FAQ.html)
- [Record TTL and Evictions](https://discuss.aerospike.com/t/records-ttl-and-evictions/737)
