
# Aerospike::predicateContains

Aerospike::predicateContains - helper method for building a contains WHERE
predicate

## Description

```
public array Aerospike::predicateContains ( string $bin, int $index_type, int|string $val )
```

**Aerospike::predicateContains()** will return an array that can be passed as the
*$where* arguement in **Aerospike::query()**.

Similar to predicateEquals(), predicateContains() looks for an exact match of a
value inside a complex type - a list containing the value (if the index type is
*INDEX_TYPE_LIST*), the value contained in the keys of a map (if the index type
is *INDEX_TYPE_MAPKEYS*), or a record with the given value contained in the
values of a map (if the index type was *INDEX_TYPE_MAPVALUES*).

## Parameters

**bin** the bin name operand

**index_type** one of *Aerospike::INDEX_TYPE_\** .

**val** the string or integer value

## Return Values

Returns an array with the following structure:
```
Associative Array:
  bin => bin name
  op => Aerospike::OP_CONTAINS
  val => scalar integer/string value
```
or *NULL* on failure.

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$where = Aerospike::predicateContains("movies", Aerospike::INDEX_TYPE_MAPKEYS, "12 Monkeys");
var_dump($where);

?>
```

We expect to see:

```
array(4) {
  ["bin"]=>
  string(6) "movies"
  ["index_type"]=>
  int(2)
  ["op"]=>
  string(8) "CONTAINS"
  ["val"]=>
  string(10) "12 Monkeys"
}
```

