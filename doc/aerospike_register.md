
# Aerospike::register

Aerospike::register - registers a UDF module with the Aerospike DB

## Description

```
public int Aerospike::register ( string $path, string $module [, int $language = Aerospike::UDF_TYPE_LUA [, array $options ]] )
```

**Aerospike::register()** will register a UDF module named *module* with the
Aerospike DB. **Note** that modules containing stream UDFs need to also be
copied to the path described in `aerospike.udf.lua_user_path`, as the last reduce
iteration is run locally on the client (after reducing on all the nodes of the
cluster).

Currently the only UDF *language* supported is Lua.  See the
[UDF Developer Guide](http://www.aerospike.com/docs/udf/udf_guide.html) on the Aerospike website.

## Parameters

**path** to the module on the client side.

**module** the name of the UDF module registered against the Aerospike DB.

**language** one of *Aerospike::UDF_TYPE_\**

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**

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

$status = $client->register('/path/to/my_udf.lua', 'my_udf.lua');
if ($status == Aerospike::OK) {
    echo "UDF module at $path is registered as my_udf on the Aerospike DB.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
UDF module at /path/to/my_udf.lua is registered as my_udf on the Aerospike DB.
```

