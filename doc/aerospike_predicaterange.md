
# Aerospike::predicateRange

Aerospike::predicateRange - helper method for building the range WHERE
predicate

## Description

```
public array Aerospike::predicateRange ( string $bin, int $index_type, int $min, int $max )
```

**Aerospike::predicateRange()** will return an array that can be passed as the
*$where* arguement in **Aerospike::query()**.

Similar to predicateBetween(), predicateRange() looks for records with a range
of values inside a complex type - a list containing the values (if the index type is
*INDEX_TYPE_LIST*), the values contained in the keys of a map (if the index type
is *INDEX_TYPE_MAPKEYS*), or a record with the given values contained in the
values of a map (if the index type was *INDEX_TYPE_MAPVALUES*).

## Parameters

**bin** the bin name operand

**index_type** one of *Aerospike::INDEX_TYPE_\** .

**min** the integer value to start with

**max** the integer value to end with

## Return Values

Returns an array with the following structure:
```
Associative Array:
  bin => bin name
  op => Aerospike::OP_RANGE
  val => array
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

// assume that the bin 'movies' contains a map of title => views
$where = Aerospike::predicateRange("movies", Aerospike::INDEX_TYPE_MAPVALUES, 10, 999);
var_dump($where);

?>
```

We expect to see:

```
array(4) {
  ["bin"]=>
  string(6) "movies"
  ["index_type"]=>
  int(3)
  ["op"]=>
  string(5) "RANGE"
  ["val"]=>
  array(2) {
    [0]=>
    int(10)
    [1]=>
    int(999)
  }
}
```

