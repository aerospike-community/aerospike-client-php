
# Aerospike::increment

Aerospike::increment - increments a numeric value in a bin

## Description

```
public int Aerospike::increment ( array $key, string $bin, int $offset [, array $options ] )
```

**Aerospike::increment()** will increment a *bin* containing a numeric value by the *offset* or
set it as the initial value if the record/bin does not exist.

If a record with the given key does not exist it will be initialized with one
bin named *bin* set to the integer value *offset* (the so-called 'upsert').


## Parameters

**key** the key under which the record can be found. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**bin** the name of the bin in which we have a numeric value.

**offset** the integer by which to increment the value in the bin.

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**
- **Aerospike::OPT_TTL**
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

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = $client->initKey("test", "users", 1234);
$options = [Aerospike::OPT_TTL => 7200];
$status = $client->increment($key, 'pto', -4, $options);
if ($status == Aerospike::OK) {
    echo "Decremented four vacation days from the user's PTO balance.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
Decremented four vacation days from the user's PTO balance.
```

