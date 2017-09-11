
# Aerospike::predicateEquals

Aerospike::predicateEquals - helper method for building an equals WHERE
predicate

## Description

```
public array Aerospike::predicateEquals ( string $bin, int|string $val )
```

**Aerospike::predicateEquals()** will return an array that can be passed as the
*$where* arguement in **Aerospike::query()**.

## Parameters

**bin** the bin name operand

**val** the string or integer value

## Return Values

Returns an array with the following structure:
```
Associative Array:
  bin => bin name
  op => Aerospike::OP_EQ
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

$where = Aerospike::predicateEquals("age", 30);
var_dump($where);

?>
```

We expect to see:

```
array(3) {
  ["bin"]=>
  string(3) "age"
  ["op"]=>
  string(1) "="
  ["val"]=>
  int(30)
}
```

