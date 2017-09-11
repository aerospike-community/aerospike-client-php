
# Aerospike::getRegistered

Aerospike::getRegistered - gets the code for a UDF module registered with the server

## Description

```
public int Aerospike::getRegistered ( string $module, string &$code [, int $language = Aerospike::UDF_TYPE_LUA [, array $options ]] )
```

**Aerospike::getRegistered()** populates *code* with the content of the matching
UDF *module* that was previously registered with the server.

## Parameters

**module** the name of the UDF module to get from the server.

**code** filled by the content of the UDF **module**

**language** one of Aerospike::UDF\_TYPE\_*.

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$status = $client->getRegistered('my_udf', $code);
if ($status == Aerospike::OK) {
    var_dump($code);
} if ($status == Aerospike::ERR_LUA_FILE_NOT_FOUND) {
    echo "The UDF module my_udf was not found to be registered with the server.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
string(351) "function startswith(rec, bin_name, prefix)
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
"
```

