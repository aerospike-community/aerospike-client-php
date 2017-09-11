
# Aerospike::put

Aerospike::put - writes a record to the Aerospike database

## Description

```
public int Aerospike::put ( array $key, array $bins [, int $ttl = 0 [, array $options ]] )
```

**Aerospike::put()** will write a record with a given *key* with *bins*
described as an array of bin-name => value pairs.
The *ttl* parameter can be used to control the expiration of the record.

By default the **Aerospike::put()** method behaves in a set-and-replace mode similar to
array keys and values. This behavior can be modified using the
*options* parameter.

**Note:** a binary-string which includes a null-byte will get truncated at the
position of the **\0** character if it is not wrapped. For more information and
the workaround see
[Handling Unsupported Types](https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#handling-unsupported-types).

## Parameters

**key** the key under which to store the record. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**bins** the array of bin names and values to write. **Bin names cannot be longer than 14 characters.** Binary data containing the null byte (**\0**) may get truncated. See the [README](https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#handling-unsupported-types) for more details and a workaround.

**ttl** the [time-to-live](http://www.aerospike.com/docs/client/c/usage/kvs/write.html#change-record-time-to-live-ttl) in seconds for the record.

**[options](aerospike.md)** including
- **Aerospike::OPT_SERIALIZER**.
- **Aerospike::OPT_WRITE_TIMEOUT**
- **[Aerospike::OPT_POLICY_RETRY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9730980a8b0eda8ab936a48009a6718)**
- **[Aerospike::OPT_POLICY_KEY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9c8a79b2ab9d3812876c3ec5d1d50ec)**
- **[Aerospike::OPT_POLICY_GEN](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga38c1a40903e463e5d0af0141e8c64061)**
- **[Aerospike::OPT_POLICY_EXISTS](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga50b94613bcf416c9c2691c9831b89238)**
- **[Aerospike::OPT_POLICY_COMMIT_LEVEL](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga17faf52aeb845998e14ba0f3745e8f23)**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

### Example #1 Aerospike::put() default behavior example

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = $client->initKey("test", "users", 1234);
$bins = ["email" => "hey@example.com", "name" => "Hey There"];
// will ensure a record exists at the given key with the specified bins
$status = $client->put($key, $bins);
if ($status == Aerospike::OK) {
    echo "Record written.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

// Updating the record
$bins = ["name" => "You There", "age" => 33];
// will update the name bin, and create a new 'age' bin
$status = $client->put($key, $bins);
if ($status == Aerospike::OK) {
    echo "Record updated.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
Record written.
Record updated.
```

### Example #2 Fail unless the put explicitly creates a new record

```php
<?php

// This time we expect an error due to the record already existing (assuming we
// already ran Example #1)
$status = $client->put($key, $bins, 0, [Aerospike::OPT_POLICY_EXISTS => Aerospike::POLICY_EXISTS_CREATE]);

if ($status == Aerospike::OK) {
    echo "Record written.\n";
} elseif ($status == Aerospike::ERR_RECORD_EXISTS) {
    echo "The Aerospike server already has a record with the given key.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}
?>
```

We expect to see:

```
The Aerospike server already has a record with the given key.
```


### Example #3 Fail if the record has been written recently

```php
<?php

// Get the record metadata and note its generation
$client->exists($key, $metadata);
$gen = $metadata['generation'];
$gen_policy = [Aerospike::POLICY_GEN_EQ, $gen];
$res = $client->put($key, $bins, 0, [Aerospike::OPT_POLICY_GEN => $gen_policy]);

if ($res == Aerospike::OK) {
    echo "Record written.\n";
} elseif ($res == Aerospike::ERR_RECORD_GENERATION) {
    echo "The record has been written since we last read it.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}
?>
```

We expect to see:

```
The record has been written since we last read it.
```

### Example #4 Handling binary strings:

```php
<?php
$str = 'Glagnar\'s Human Rinds, "It\'s a bunch\'a munch\'a crunch\'a human!';
$deflated = new \Aerospike\Bytes(gzdeflate($str));
$wrapped = new \Aerospike\Bytes("trunc\0ated");

$key = $client->initKey('test', 'demo', 'wrapped-bytes');
$status = $client->put($key, ['unwrapped'=>"trunc\0ated", 'wrapped'=> $wrapped, 'deflated' => $deflated]);
if ($status !== Aerospike::OK) {
    die($client->error());
}
$client->get($key, $record);
$wrapped = \Aerospike\Bytes::unwrap($record['bins']['wrapped']);
$deflated = $record['bins']['deflated'];
$inflated = gzinflate($deflated->s);
echo "$inflated\n";
echo "wrapped binary-string: ";
var_dump($wrapped);
$unwrapped = $record['bins']['unwrapped'];
echo "The binary-string that was given to put() without a wrapper: $unwrapped\n";
```
We expect to see:
```
Glagnar's Human Rinds, "It's a bunch'a munch'a crunch'a human!
wrapped binary-string: string(10) "truncated"
The binary-string that was given to put() without a wrapper: trunc
```

## See Also

- [Aerospike Data Model](http://www.aerospike.com/docs/architecture/data-model.html)
- [Key-Value Store](http://www.aerospike.com/docs/guide/kvs.html)
- [Glossary](http://www.aerospike.com/docs/guide/glossary.html)
