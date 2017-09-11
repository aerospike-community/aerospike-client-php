
# Aerospike::aggregate

Aerospike::aggregate - Applies a stream UDF to a secondary index query

## Description

```
public int Aerospike::aggregate ( string $ns, string $set, array $where, string $module, string $function, array $args, mixed &$returned [, array $options ] )
```

**Aerospike::aggregate()** will apply the stream UDF *module*.*function* with
*args* to the result of running a secondary index query on *ns*.*set*.
The aggregated *returned* variable is then filled, with its type depending on the UDF.
It may be a string, integer or array, and potentially an array of
those, such as in the case the UDF does not specify a reducer and there are
multiple nodes in the cluster (each sending back the result of its own
aggregation).

As with query(), if an empty array is given as the *where* predicate a 'scan
aggregation' is initiated instead of a query, which means the stream UDF is
applied to all the records returned by the scan.

**Note** that modules containing stream UDFs need to also be
copied to the path described in `aerospike.udf.lua_user_path`, as the last reduce
iteration is run locally on the client (after reducing on all the nodes of the
cluster).

**Note** aggregate is currently unsupported in PHP built with ZTS enabled. Attempting to use it in that environment
will fail.

Currently the only UDF language supported is Lua.  See the
[UDF Developer Guide](http://www.aerospike.com/docs/udf/udf_guide.html) on the Aerospike website.

## Parameters

**ns** the namespace

**set** the set to be queried

**where** the predicate for the query, conforming to one of the following:
These are usually created by the [predicate methods](aerospike_predicateequals.md)
```
Array:
  bin => bin name
  op => one of Aerospike::OP_EQ, Aerospike::OP_BETWEEN
  val => scalar integer/string for OP_EQ or [$min, $max] for OP_BETWEEN
```
*examples:*
```
["bin"=>"name", "op"=>Aerospike::OP_EQ, "val"=>"foo"]
["bin"=>"age", "op"=>Aerospike::OP_BETWEEN, "val"=>[35,50]]
```

**module** the name of the UDF module registered against the Aerospike DB.

**function** the name of the function to be applied to the record stream.

**args** an array of arguments for the UDF.

**returned** filled by one or more of the supported types.

**[options](aerospike.md)** including
- Aerospike::OPT_READ_TIMEOUT

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

### Example Stream UDF

Registered module **stream_udf.lua**
```lua
local function having_ge_threshold(bin_having, ge_threshold)
    debug("group_count::thresh_filter: %s >  %s ?", tostring(rec[bin_having]), tostring(ge_threshold))
    return function(rec)
        if rec[bin_having] < ge_threshold then
            return false
        end
        return true
    end
end

local function count(group_by_bin)
  return function(group, rec)
    if rec[group_by_bin] then
      local bin_name = rec[group_by_bin]
      group[bin_name] = (group[bin_name] or 0) + 1
    end
    return group
  end
end

local function add_values(val1, val2)
  return val1 + val2
end

local function reduce_groups(a, b)
  return map.merge(a, b, add_values)
end

function group_count(stream, group_by_bin, bin_having, ge_threshold)
  if bin_having and ge_threshold then
    local myfilter = having_ge_threshold(bin_having, ge_threshold)
    return stream : filter(myfilter) : aggregate(map{}, count(group_by_bin)) : reduce(reduce_groups)
  else
    return stream : aggregate(map{}, count(group_by_bin)) : reduce(reduce_groups)
  end
end
```


### Example of aggregating a stream UDF to the result of a secondary index query
```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]]];
$client = new Aerospike($config);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

// assuming test.users has a bin first_name, show the first name distribution
// for users in their twenties
$where = Aerospike::predicateBetween("age", 20, 29);
$status = $client->aggregate("test", "users", $where, "stream_udf", "group_count", ["first_name"], $names);
if ($status == Aerospike::OK) {
    var_dump($names);
} else {
    echo "An error occured while running the AGGREGATE [{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(5) {
  ["Claudio"]=>
  int(1)
  ["Michael"]=>
  int(3)
  ["Jennifer"]=>
  int(2)
  ["Jessica"]=>
  int(3)
  ["Jonathan"]=>
  int(3)
}
```

## See Also

- [Aerospike::predicateEquals()](aerospike_predicateequals.md)
- [Aerospike::predicateBetween()](aerospike_predicatebetween.md)
- [Query](http://www.aerospike.com/docs/guide/query.html)
- [Aggregations](http://www.aerospike.com/docs/guide/aggregation.html)
- [Developing Stream UDFs](http://www.aerospike.com/docs/udf/developing_stream_udfs.html)
