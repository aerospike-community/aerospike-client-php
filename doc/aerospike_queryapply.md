
# Aerospike::queryApply

Aerospike::queryApply - Apply a record UDF to each record matched by a query
being executed in the background.

## Description

```
public int Aerospike::queryApply ( string $ns, string $set, array $where, string $module, string $function, array $args, int &$job_id [, array $options ] )
```

**Aerospike::queryApply()** will initiate a background query and apply a record UDF
*module*.*function* with *args* to each record matching the predicate in *ns*.*set*.

An integer *job_id* identifies the background query for subsequent **jobInfo()**
inquiries. As **queryApply()** is performed in the background, no results will be
returned to the client.

Currently the only UDF language supported is Lua.  See the
[UDF Developer Guide](http://www.aerospike.com/docs/udf/udf_guide.html) on the Aerospike website.

## Parameters

**ns** the namespace

**set** the set to be queried

**where** the predicate conforming to one of the following:
```
Associative Array:
  bin => bin name
  op => one of Aerospike::OP_EQ, Aerospike::OP_BETWEEN, Aerospike::OP_CONTAINS, Aerospike::OP_RANGE
  val => scalar integer/string for OP_EQ or array($min, $max) for OP_BETWEEN

or an empty array() for no predicate.
```

**module** the name of the UDF module registered against the Aerospike DB.

**function** the name of the function to be applied to the records.

**args** an array of arguments for the UDF.

**job_id** filled by an integer handle for the initiated background query.

**options** including
- **Aerospike::OPT_WRITE_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

### Example Record UDF

Registered module **my_udf.lua**
```lua
function mytransform(rec, offset)
    rec['a'] = rec['a'] + offset
    rec['b'] = rec['a'] * offset
    rec['c'] = rec['a'] + rec['b']
    aerospike:update(rec)
end
```

### Example of initiating a background query and applying a record UDF

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

// assuming test.users has integer bins 'a', 'b' and 'c'.
// Adds offset to the value in bin 'a', multiplies the value in bin 'b' by offset and
// Updates value in bin 'c' with the sum of updated values in bins 'a' and 'b'.

$where = Aerospike::predicateBetween("age", 30, 39);
$status = $client->queryApply("test", "users", $where, "my_udf", "mytransform", [20], $job_id);
if ($status === Aerospike::OK) {
    var_dump("The job ID is $job_id");
} else if ($status === Aerospike::ERR_CLIENT) {
    echo "An error occured while initiating the background query [{$client->errorno()}] ".$client->error();
} else {
    echo "An error occured while running the background query [{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
string(12) "The job ID is 1"
```

## See Also

- [Aerospike::jobInfo()](aerospike_jobinfo.md)
- [Aerospike::predicateEquals()](aerospike_predicateequals.md)
- [Aerospike::predicateContains()](aerospike_predicatecontains.md)
- [Aerospike::predicateBetween()](aerospike_predicatebetween.md)
- [Aerospike::predicateRange()](aerospike_predicaterange.md)
- [Query](http://www.aerospike.com/docs/guide/query.html)
- [UDF Developer Guide](http://www.aerospike.com/docs/udf/udf_guide.html)
- [Developing Record UDFs](http://www.aerospike.com/docs/udf/developing_record_udfs.html)
- [Lua UDF - API Reference](http://www.aerospike.com/docs/udf/api_reference.html)

