
# Aerospike::query

Aerospike::query - queries a secondary index on a set in the Aerospike database

## Description

```
public int Aerospike::query ( string $ns, string $set, array $where, callback
        $record_cb [, array $select [, array $options ]] )
```

**Aerospike::query()** will query a *set* with a specified *where* predicate
then invoke a callback function *record_cb* on each record in the result stream.
The bins returned can be filtered by passing an associative array of the *bins*
needed, otherwise all bins in the record are returned (similar to a SELECT \*).
Non-existent bins will appear in the *record* with a NULL value.

To query **without a predicate** the value of the *where* must be an empty
array. As a result, a scan is initiated instead of a query.

## Parameters

**ns** the namespace

**set** the set to be queried

**where** the predicate conforming to one of the following:
```
Associative Array:
  bin => bin name
  op => one of Aerospike::OP_EQ, Aerospike::OP_BETWEEN, Aerospike::OP_CONTAINS, Aerospike::OP_RANGE
  val => scalar integer/string for OP_EQ or [$min, $max] for OP_BETWEEN

or an empty array() for no predicate.
```

*examples:*
```
["bin"=>"name", "op"=>Aerospike::OP_EQ, "val"=>"foo"]
["bin"=>"age", "op"=>Aerospike::OP_BETWEEN, "val"=>[35,50]]
["bin"=>"movies", "op"=>Aerospike::OP_CONTAINS, "val"=>"12 Monkeys"]
["bin"=>"movies", "op"=>Aerospike::OP_RANGE, "val"=>[10,1000]]
[] // no predicate
```

**record_cb** a callback function invoked for each [record](aerospike_get.md#parameters) streaming back from the server.

**select** an array of bin names which are the subset to be returned.

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

### Buffer the records returned from the query

```php
<?php

$result = [];
$where = Aerospike::predicateBetween("age", 30, 39);
$status = $client->query("test", "users", $where, function ($record) use (&$result) {
    $result[] = $record['bins'];
});
if ($status !== Aerospike::OK) {
    echo "An error occured while querying[{$client->errorno()}] {$client->error()}\n";
} else {
    echo "The query returned ".count($result)." records\n";
}

?>
```


### Using the external scope

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$total = 0;
$in_thirties = 0;
$where = Aerospike::predicateBetween("age", 30, 39);
$status = $client->query("test", "users", $where, function ($record) use (&$total, &$in_thirties) {
    echo "{$record['bins']['email']} age {$record['bins']['age']}\n";
    $total += (int) $record['bins']['age'];
    $in_thirties++;
}, ["email", "age"]);
if ($status == Aerospike::ERR_QUERY) {
    echo "An error occured while querying[{$client->errorno()}] {$client->error()}\n";
} else {
    echo "The average age of employees in their thirties is ".round($total / $in_thirties)."\n";
}

?>
```

We expect to see:

```
foo age 32
bar age 35
The average age of employees in their thirties is 34
```

## See Also

- [Aerospike::predicateEquals()](aerospike_predicateequals.md)
- [Aerospike::predicateContains()](aerospike_predicatecontains.md)
- [Aerospike::predicateBetween()](aerospike_predicatebetween.md)
- [Aerospike::predicateRange()](aerospike_predicaterange.md)
- [Query](http://www.aerospike.com/docs/guide/query.html)
- [Managing Queries](http://www.aerospike.com/docs/operations/manage/queries/index.html)

