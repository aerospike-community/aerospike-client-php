
# Aerospike::scanApply

Aerospike::scanApply - Apply a record UDF to each record in a background scan.

## Description

```
public int Aerospike::scanApply ( string $ns, string $set, string $module, string $function, array $args, int &$job_id [, array $options ] )
```

**Aerospike::scanApply()** will initiate a background read/write scan and apply a record UDF
*module*.*function* with *args* to each record being scanned in *ns*.*set*.

An integer *job_id* identifies the background scan for subsequent **jobInfo()**
inquiries. As **scanApply()** is performed in the background,no results will be
returned to the client.

Currently the only UDF language supported is Lua.  See the
[UDF Developer Guide](http://www.aerospike.com/docs/udf/udf_guide.html) on the Aerospike website.

## Parameters

**ns** the namespace

**set** the set to be scanned

**module** the name of the UDF module registered against the Aerospike DB.

**function** the name of the function to be applied to the records.

**args** an array of arguments for the UDF.

**job_id** filled by an integer handle for the initiated background scan

**options** including
- **Aerospike::OPT_WRITE_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## See Also

- [Aerospike::jobInfo()](aerospike_jobinfo.md)

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

### Example of initiating a background read/write scan by applying a record UDF

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

$status = $client->scanApply("test", "users", "my_udf", "mytransform", array(20), $job_id);
if ($status === Aerospike::OK) {
    var_dump("scan ID is $job_id");
} else if ($status === Aerospike::ERR_CLIENT) {
    echo "An error occured while initiating the BACKGROUND SCAN [{$client->errorno()}] ".$client->error();
} else {
    echo "An error occured while running the BACKGROUND SCAN [{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
string(12) "scan ID is 1"
```

