
# Aerospike::predicateBetween

Aerospike::predicateBetween - helper method for building the between WHERE
predicate

## Description

```
public array Aerospike::predicateBetween ( string $bin, int $min, int $max )
```

**Aerospike::predicateBetween()** will return an array that can be passed as the
*$where* arguement in **Aerospike::query()**.

## Parameters

**bin** the bin name operand

**min** the integer value to start with

**max** the integer value to end with

## Return Values

Returns an array with the following structure:
```
Associative Array:
  bin => bin name
  op => Aerospike::OP_BETWEEN
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

$where = Aerospike::predicateBetween("age", 30, 39);
var_dump($where);

?>
```

We expect to see:

```
array(3) {
  ["bin"]=>
  string(3) "age"
  ["op"]=>
  string(1) "BETWEEN"
  ["val"]=>
  array(2) {
    [0]=>
    int(30)
    [1]=>
    int(39)
  }
}
```

