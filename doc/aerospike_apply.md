
# Aerospike::apply

Aerospike::apply - Applies a UDF to a record at the Aerospike DB

## Description

```
public int Aerospike::apply ( array $key, string $module, string $function[, array $args [, mixed &$returned [, array $options ]]] )
```

**Aerospike::apply()** will apply the UDF *module*.*function* to a record with
a given *key*. A list of *args* can be passed to the UDF and any *returned*
value captured optionally.

Currently the only UDF language supported is Lua.  See the
[UDF Developer Guide](http://www.aerospike.com/docs/udf/udf_guide.html) on the Aerospike website.

## Parameters

**key** the key under which to store the record. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**module** the name of the UDF module registered against the Aerospike DB.

**function** the name of the function to be applied to the record.

**args** an array of arguments for the UDF.

**returned** if passed will contain the result value (integer, string, array) of
calling the UDF.

**[options](aerospike.md)** including
- **Aerospike::OPT_POLICY_KEY**
- **Aerospike::OPT_WRITE_TIMEOUT**
- **Aerospike::OPT_SERIALIZER**.

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

### Example Record UDF

Registered module **my_udf.lua**
```lua
function startswith(rec, bin_name, prefix)
  if not aerospike:exists(rec) then
    return false
  end
  if not prefix then
    return true
  end
  if not rec[bin_name] then
    return false
  end
  local bin_val = rec[bin_name]
  l = prefix:len()
  if l > bin_val:len() then
    return false
  end
  ret = bin_val:sub(1, l) == prefix
  return ret
end
```

## Example of applying the UDF on a record

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = ["ns" => "test", "set" => "users", "key" => "1234"];
$status = $client->apply($key, 'my_udf', 'startswith', ['email', 'hey@'], $returned);
if ($status == Aerospike::OK) {
    if ($returned) {
        echo "The email of the user with key {$key['key']} starts with 'hey@'.\n";
    } else {
        echo "The email of the user with key {$key['key']} does not start with 'hey@'.\n";
    }
} elseif ($status == Aerospike::ERR_UDF_NOT_FOUND) {
    echo "The UDF module my_udf.lua was not registered with the Aerospike DB.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

If the user with the specified key had an email beginning with 'hey@' We expect to see:

```
The email of the user with key 1234 starts with 'hey@'.
```

